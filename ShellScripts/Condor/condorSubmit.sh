#!/bin/bash

if [ $# -ne 6 ];
then
  echo "Usage: ./ShellScripts/Condor/condorSubmit.sh <progPath> <configFile> <inputFiles> <output dir> <nJobs> <nFilesPerJob>"
  echo "<inputFiles> = list of input files"
  echo "<nJobs> = total number of jobs"
  echo "<nFilesPerJob> = number of input files per job"
  exit 1
fi

progPath=$1
configFile=$2
inputList=$3
outputDir=$4
nJobs=$5
nFilesPerJob=$6  # number files to be processed in a single job, take care to split your file so that you run on all files. The last job might be with smaller number of files (the ones that remain).

echo "program path      : $progPath"
echo "coniguration file : $configFile"
echo "input file        : $inputList"
echo "output directory  : $outputDir"

if [[ $outputDir != /mnt/hadoop/* ]]; then
    echo "output directory must be under /mnt/hadoop/"
    exit 1
fi

# calculate/check nJobs and nFilesPerJob
nFiles=$(cat $inputList | wc -l)
echo "Total input files : $nFiles"

if [ $nFilesPerJob -lt 1 ]; then
  echo "calculating the number of files per job based on nJobs"
  nFilesPerJob=$((($nFiles-1)/$nJobs + 1))
elif [ $nJobs -lt 1 ]; then
  echo "calculating the number of jobs based on interval"
  nJobs=$((($nFiles-1)/$nFilesPerJob + 1))
fi

echo "Number of jobs : $nJobs"
echo "Number of files per job : $nFilesPerJob"

# check sanity of nJobs and nFilesPerJob
nFilesToProcess=$(($nJobs * $nFilesPerJob))
if [ "$nFilesToProcess" -lt "$nFiles" ]; then
  echo "Warning : number of files to process is $nFilesToProcess"
  echo "Not all the files will be processed."
fi

# create the directories for condor submission and condor output files
baseDir=$PWD
timeNow=$(date +"%Y%m%d_%H%M%S")
submitDir=$baseDir"/condorSubmissions/"$timeNow
condorLogsDir=$baseDir"/condorLogs/"$timeNow
mkdir -p $submitDir
mkdir -p $condorLogsDir

echo "directory for condor submission : $submitDir" 
echo "directory for condor output     : $condorLogsDir" 

cp $progPath $submitDir
cp $configFile $submitDir
cp $inputList $submitDir
cp $PWD"/ShellScripts/myRun.sh" $submitDir

progName=$(basename $progPath)
configName=$(basename $configFile)
inputListName=$(basename $inputList)

## customizations for submit-hi2.mit.edu and submit.mit.edu machines ##
# proxy files start with "x509" and they are located under /tmp/ only.
proxyFilePath=$(find /tmp/ -maxdepth 1 -user $USER  -type f -name "x509*" -print | head -1)
proxyFile=$(basename $proxyFilePath)

srmPrefix="/mnt/hadoop/"
outputDirSRM=${outputDir#${srmPrefix}}

gfal-mkdir -p gsiftp://se01.cmsaf.mit.edu:2811/${outputDirSRM}
## customizations for submit-hi2.mit.edu and submit.mit.edu machines - END ##

# create the "submit description file"
cat > $submitDir/submit.condor <<EOF

Universe     = vanilla
Initialdir   = $submitDir
Notification = Error
Executable   = $submitDir/condorExecutable.sh
Arguments    = \$(Process) $nFilesPerJob $progName $configName $inputListName $outputDir
GetEnv       = True
Output       = $condorLogsDir/\$(Process).out
Error        = $condorLogsDir/\$(Process).err
Log          = $condorLogsDir/\$(Process).log
Rank         = Mips
+AccountingGroup = "group_cmshi.$USER"
requirements = GLIDEIN_Site == "MIT_CampusFactory" && BOSCOGroup == "bosco_cmshi" && HAS_CVMFS_cms_cern_ch && BOSCOCluster == "ce03.cmsaf.mit.edu"
job_lease_duration = 240
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
transfer_input_files = /tmp/$proxyFile,myRun.sh,$progName,$configName,$inputListName

Queue $nJobs

EOF

# create the "executable" file
cat > $submitDir/condorExecutable.sh <<EOF
#!/bin/bash

jobIndex=\$1
nFilesTmp=\$2
inputListTmp="inputList_"\$jobIndex".txt"
outputTmp="job"\$jobIndex".root"

progExe=\$3
configTmp=\$4
outputDirTmp=\$6

# setup grid proxy
export X509_USER_PROXY=\${PWD}/$proxyFile
# set hadoop directory path for grid tools
srmPrefix="/mnt/hadoop/"
outputDirSRM=\${outputDirTmp#\${srmPrefix}}

lineStart=\$((\$jobIndex * \$nFilesTmp + 1))
lineEnd=\$((\$lineStart + \$nFilesTmp - 1))
#echo "lineStart : \$lineStart"
#echo "lineEnd   : \$lineEnd"
sed "\$lineStart,\$lineEnd!d" \$5 > \$inputListTmp

echo "##"
echo "host : \$(hostname)"
echo "PWD  : \$PWD"
echo "##"
./myRun.sh "./"\$progExe \$configTmp \$inputListTmp \$outputTmp

echo "## directory content ##"
ls -altrh
echo "## directory content - END ##"

set -x
mv \$outputTmp \$outputDirTmp

# $? is the exit status of last run command
if [ \$? -ne 0 ]; then
  gfal-copy file://\${PWD}/\${outputTmp} gsiftp://se01.cmsaf.mit.edu:2811/\${outputDirSRM}/\${outputTmp}

  if [ \$? -ne 0 ]; then
    srmcp -2 \$outputTmp gsiftp://se01.cmsaf.mit.edu:2811/\${outputDirSRM}/\${outputTmp}
  fi
fi
set +x

# delete all the files but the .out, .err, and .log files
ls | grep -v -e ".out" -e ".err" -e ".log" -e "_condor_stdout" -e "_condor_stderr" | xargs rm -rfv

echo "## directory content after deletion ##"
ls -altrh
echo "## directory content after deletion - END ##"

EOF

# create a script to merge the output after the jobs finish
cat > $submitDir/mergeOutput.sh <<EOF
#!/bin/bash

jobOutput=$outputDir"/"job*.root

hadoopBase="/mnt/hadoop/cms/store/user/"$USER
scratchBase="/export/d00/scratch/"$USER

# merge the output only if a scratch directory exists for that user.
if [ ! -d "\$scratchBase" ]; then
  echo "scratch directory does not exist : \$scratchBase"
  exit 1
fi

dirTmp=\$(echo $outputDir | sed "s#\$hadoopBase#\$scratchBase#g")
mergedOutputName=\$(basename \$dirTmp)".root"
mergedOutDir=\$(dirname \$dirTmp)
mergedOutput=\$mergedOutDir"/"\$mergedOutputName
# if $dirTmp is "my/merged/output/", then $mergedOutput is "my/merged/output.root"

mkdir -p \$mergedOutDir
hadd -f \$mergedOutput \$jobOutput

EOF
chmod u+x $submitDir/mergeOutput.sh

# create a simple script to delete the directory with job outputs
cat > $submitDir/rmJobOutput.sh <<EOF
#!/bin/bash

rm -r $outputDir

EOF
chmod u+x $submitDir/rmJobOutput.sh

cat $submitDir/submit.condor
echo "condor_submit $submitDir/submit.condor -pool submit.mit.edu:9615 -name submit.mit.edu -spool" 
