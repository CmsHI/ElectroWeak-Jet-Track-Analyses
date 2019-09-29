#!/bin/bash

source ./ShellScripts/bashUtils.sh

if [ $# -ne 8 ];
then
  echo "Usage: ./ShellScripts/vJetTrk/Condor/condorSubmit.skim.sh <progPath> <configFile> <inputFiles> <output dir> <mixFilesList> <optsFile> <nJobs> <nFilesPerJob>"
  echo "<inputFiles> = list of input files"
  echo "<nJobs> = total number of jobs"
  echo "<nFilesPerJob> = number of input files per job"
  exit 1
fi

progPath=$1
configFile=$2
inputList=$3       # a ROOT file or a file with list of ROOT files
outputDir=$4
mixFilesList=$5    # a file with list of ROOT files
optsFile=$6  # text file containing program options
nJobs=$7
nFilesPerJob=$8  # number files to be processed in a single job, take care to split your file so that you run on all files. The last job might be with smaller number of files (the ones that remain).

echo "program path      : "$progPath
echo "coniguration file : "$configFile
echo "input list        : "$inputList
echo "output directory  : "$outputDir
echo "mix file list     : "$mixFilesList
echo "file with options : "$optsFile
echo "nJobs             : "$nJobs
echo "nFilesPerJob      : "$nFilesPerJob

# check the machine on which this script is run. Currently the script works on lxplus machines and submit.mit.edu. In this context submit.mit.edu includes submit-hi1.mit.edu and submit-hi2.mit.edu
[[ $HOSTNAME = lxplus*.cern.ch ]] && isLxplus=1 || isLxplus=0
[[ $HOSTNAME = submit*.mit.edu ]] && isSubmitMIT=1 || isSubmitMIT=0
echo "Host is ${HOSTNAME} ."
if [ $isLxplus -gt 0 ]; then
  echo "This is an lxplus machine."
elif [ $isSubmitMIT -gt 0 ]; then
  echo "This is a submit.mit.edu machine."
else
  echo "The script cannot run on this machine. Exiting."
  exit 1
fi

if [[ $isLxplus -gt 0 && ($outputDir != /afs/* && $outputDir != /eos/*) ]]; then
  echo "output directory must be under /afs or /eos"
  exit 1
elif [[ $isSubmitMIT -gt 0 && $outputDir != /mnt/hadoop/* ]]; then
  echo "output directory must be under /mnt/hadoop/"
  exit 1
fi

timeNow=$(date +"%Y%m%d_%H%M%S")
isTmpList=0
if [[ $inputList = *.root ]]; then
    isTmpList=1
    inputFileTmp=$inputList
    inputListTmp="tmpList_"$timeNow".list"
    echo $inputFileTmp > $inputListTmp
    inputList=$inputListTmp
    echo "input list was given as a ROOT file, not as a list of ROOT files. Created temporary list file : "$inputListTmp
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
baseDir=""
submitDir=""
condorLogsDir=""
if [ $isLxplus -gt 0 ]; then
  firstLetter=${USER:0:1}
  baseDir="/afs/cern.ch/work/"$firstLetter"/"$USER"/public/ewjta"
  submitDir=$baseDir"/condorSubmissions/vJetTrk/"$timeNow
  condorLogsDir=$submitDir"/logs"
elif [ $isSubmitMIT -gt 0 ]; then
  baseDir="/work/"$USER"/ewjta"
  submitDir=$baseDir"/condorSubmissions/vJetTrk/"$timeNow
  condorLogsDir=$submitDir"/logs"
fi
mkdir -p $submitDir
mkdir -p $condorLogsDir

echo "directory for condor submission : $submitDir" 
echo "directory for condor output     : $condorLogsDir" 

cp $progPath $submitDir
cp $configFile $submitDir
cp $inputList $submitDir


[[ $mixFilesList = NOFILE ]] && useMixFileList=0 || useMixFileList=1

mixFilesListName="mixFiles.list"
touch $submitDir"/"${mixFilesListName}
if [ $useMixFileList -gt 0 ]; then
  cp $mixFilesList $submitDir"/"${mixFilesListName}
else
  echo "This program will not use mix events."
fi

cp $optsFile $submitDir
cp $PWD"/ShellScripts/myRun.sh" $submitDir

if [ $isTmpList -gt 0 ]; then
  rm $inputList
fi

progName=$(basename $progPath)
configName=$(basename $configFile)
inputListName=$(basename $inputList)
optsFileName=$(basename $optsFile)

# find and copy the imported configuration files
configListTmp="importedConfigs.list"
configsAll="allConfigs.tar"
rm -f $configListTmp
touch $configListTmp
findImportedConfigs $configFile $configListTmp
tar -cvf $configsAll -T $configListTmp
cp $configsAll $submitDir
rm -f $configListTmp
rm -f $configsAll

# copy the correction files
corrFileList="corrFiles.list"
corrFilesAll="allCorrFiles.tar"
rm -f $corrFileList
echo "Corrections/tracks/2018PbPb_TrackingEfficiencies_Prelim/2018PbPb_Efficiency_GeneralTracks_highPt.root" >> ${corrFileList}
echo "Corrections/tracks/2018PbPb_TrackingEfficiencies_Prelim/2018PbPb_Efficiency_GeneralTracks_MB.root" >> ${corrFileList}
echo "Corrections/tracks/2018PbPb_TrackingEfficiencies_Prelim/2018PbPb_Efficiency_PixelTracks.root" >> ${corrFileList}
echo "Corrections/tracks/TrackingCorrection_2017pp/2017pp_TrkCorr_Aug16_Prelim.root" >> ${corrFileList}
echo "Corrections/tracks/TrackingCorrection_2017pp/trackingEfficiency2017pp.h" >> ${corrFileList}
tar -cvf $corrFilesAll -T $corrFileList
cp $corrFilesAll $submitDir
rm -f $corrFileList
rm -f $corrFilesAll

## customizations for lxplus and submit.mit.edu machines ##
# proxy files start with "x509" and they are located under /tmp/ only.
proxyFilePath=$(find /tmp/ -maxdepth 1 -user $USER -type f -name "x509*" -print | head -1)
proxyFile=$(basename $proxyFilePath)

if [ $isLxplus -gt 0 ]; then
  cp $proxyFilePath $submitDir
fi

if [ $isLxplus -gt 0 ]; then
  mkdir -p $outputDir
elif [ $isSubmitMIT -gt 0 ]; then
  srmPrefix="/mnt/hadoop/"
  outputDirSRM=${outputDir#${srmPrefix}}

  gfal-mkdir -p gsiftp://se01.cmsaf.mit.edu:2811/${outputDirSRM}
fi
## customizations for submit-hi2.mit.edu and submit.mit.edu machines - END ##

## create the "submit description file" for Lxplus and submit.mit.edu
cat > $submitDir/submit.condor.Lxplus <<EOF

Universe     = vanilla
Initialdir   = $submitDir
Notification = Error
Executable   = $submitDir/condorExecutable.sh
Arguments    = \$(Process) $nFilesPerJob $progName $configName $inputListName $outputDir $mixFilesListName $optsFileName
GetEnv       = True
Output       = $condorLogsDir/\$(Process).out
Error        = $condorLogsDir/\$(Process).err
Log          = $condorLogsDir/\$(Process).log
Rank         = Mips
#+AccountingGroup = "group_cmshi.$USER"
#requirements = OpSysAndVer =?= "CentOS7"
+JobFlavour = "longlunch"
#+MaxRuntime = 14400 # Number of seconds
#RequestCpus = 4
#request_memory = 8000M  # The rule is that 2 GB of memory per CPU core.
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
transfer_input_files = $proxyFile,myRun.sh,$progName,$configName,$inputListName,$mixFilesListName,$optsFileName,$configsAll,$corrFilesAll

Queue $nJobs

EOF

cat > $submitDir/submit.condor.submitMIT <<EOF

Universe     = vanilla
Initialdir   = $submitDir
Notification = Error
Executable   = $submitDir/condorExecutable.sh
Arguments    = \$(Process) $nFilesPerJob $progName $configName $inputListName $outputDir $mixFilesListName $optsFileName
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
transfer_input_files = /tmp/$proxyFile,myRun.sh,$progName,$configName,$inputListName,$mixFilesListName,$optsFileName,$configsAll,$corrFilesAll

Queue $nJobs

EOF

# create the "executable" file for Lxplus and submit.mit.edu
cat > $submitDir/condorExecutable.Lxplus.sh <<EOF
#!/bin/bash

jobIndex=\$1
nFilesTmp=\$2
inputListTmp="inputList_"\$jobIndex".txt"
outputTmp="job"\$jobIndex".root"

progExe=\$3
configTmp=\$4
outputDirTmp=\$6
mixFileListTmp=""
if [ $useMixFileList -gt 0 ]; then
  mixFileListTmp=\$7
fi
optsFileTmp=\$8

optionsTmp=\$(< \${optsFileTmp} )
if [ $useMixFileList -gt 0 ]; then
  # use job index as random seed
  optionsTmp=\${optionsTmp}" --rndSeed="\${jobIndex}
fi

# setup grid proxy
export X509_USER_PROXY=\${PWD}/$proxyFile

lineStart=\$((\$jobIndex * \$nFilesTmp + 1))
lineEnd=\$((\$lineStart + \$nFilesTmp - 1))
#echo "lineStart : \$lineStart"
#echo "lineEnd   : \$lineEnd"
sed "\$lineStart,\$lineEnd!d" \$5 > \$inputListTmp

echo "##"
echo "host : \$(hostname)"
echo "PWD  : \$PWD"
echo "##"
# extract imported configuration files, if any
if [ -f $configsAll ]; then
  tar -xvf $configsAll
fi
# extract imported correction files, if any
if [ -f $corrFilesAll ]; then
  tar -xvf $corrFilesAll
fi
./myRun.sh "./"\$progExe \$configTmp \$inputListTmp \$outputTmp \$mixFileListTmp \${optionsTmp}

echo "## directory content ##"
ls -altrh
echo "## directory content - END ##"

set -x
mv -f \$outputTmp \$outputDirTmp

set +x

# delete all the files but the .out, .err, and .log files
ls | grep -v -e ".out" -e ".err" -e ".log" -e "_condor_stdout" -e "_condor_stderr" | xargs rm -rfv

echo "## directory content after deletion ##"
ls -altrh
echo "## directory content after deletion - END ##"

EOF

cat > $submitDir/condorExecutable.submitMIT.sh <<EOF
#!/bin/bash

jobIndex=\$1
nFilesTmp=\$2
inputListTmp="inputList_"\$jobIndex".txt"
outputTmp="job"\$jobIndex".root"

progExe=\$3
configTmp=\$4
outputDirTmp=\$6
mixFileListTmp=""
if [ $useMixFileList -gt 0 ]; then
  mixFileListTmp=\$7
fi
optsFileTmp=\$8

optionsTmp=\$(< \${optsFileTmp} )
if [ $useMixFileList -gt 0 ]; then
  # use job index as random seed
  optionsTmp=\${optionsTmp}" --rndSeed="\${jobIndex}
fi

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
# extract imported configuration files, if any
if [ -f $configsAll ]; then
  tar -xvf $configsAll
fi
# extract imported correction files, if any
if [ -f $corrFilesAll ]; then
  tar -xvf $corrFilesAll
fi
./myRun.sh "./"\$progExe \$configTmp \$inputListTmp \$outputTmp \$mixFileListTmp \${optionsTmp}

echo "## directory content ##"
ls -altrh
echo "## directory content - END ##"

set -x
mv -f \$outputTmp \$outputDirTmp

# $? is the exit status of last run command
if [ \$? -ne 0 ]; then
  gfal-copy -f -t 180000 -T 180000 file://\${PWD}/\${outputTmp} gsiftp://se01.cmsaf.mit.edu:2811/\${outputDirSRM}/\${outputTmp}

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

## select the condor submission file and the executable for this machine
if [ $isLxplus -gt 0 ]; then
  mv $submitDir/submit.condor.Lxplus $submitDir/submit.condor
  mv $submitDir/condorExecutable.Lxplus.sh $submitDir/condorExecutable.sh
  rm $submitDir/submit.condor.submitMIT
  rm $submitDir/condorExecutable.submitMIT.sh
elif [ $isSubmitMIT -gt 0 ]; then
  mv $submitDir/submit.condor.submitMIT $submitDir/submit.condor
  mv $submitDir/condorExecutable.submitMIT.sh $submitDir/condorExecutable.sh
  rm $submitDir/submit.condor.Lxplus
  rm $submitDir/condorExecutable.Lxplus.sh
fi

# create a script to merge the output after the jobs finish
cat > $submitDir/mergeOutput.sh <<EOF
#!/bin/bash

jobOutput=$outputDir"/"job*.root
nFiles=\$(ls -1 \$jobOutput | wc -l)

USERGRID=$(voms-proxy-info -issuer | awk '{split($0,myLine,"/CN="); print myLine[2]}')
if [ -z "\$USERGRID" ]; then
  USERGRID=$USER
fi

outputBase=""
if [[ $isLxplus -gt 0 && $outputDir == /afs/* ]]; then
  outputBase="/afs/cern.ch/work/"$firstLetter"/"$USER"/public"
elif [[ $isLxplus -gt 0 && $outputDir == /eos/* ]]; then
  outputBase="/eos/cms/store/group/phys_heavyions/"\$USER
elif [[ $isSubmitMIT -gt 0 && $outputDir == /mnt/hadoop/* ]]; then
  outputBase="/mnt/hadoop/cms/store/user/"\$USERGRID
fi

if [[ -z "\$outputBase" || ! -d "\$outputBase" ]]; then
  echo "Output directory does not exist : \$outputBase"
  exit 1
fi

# merge the output only if a scratch/work directory exists for that user.
tmpHaddDir=""
if [ $isLxplus -gt 0 ]; then
  firstLetter=${USER:0:1}
  tmpHaddDir="/afs/cern.ch/work/"$firstLetter"/"$USER"/public"
elif [ $isSubmitMIT -gt 0 ]; then
  tmpHaddDir="/export/d00/scratch/"$USER
fi

if [[ -z "\$tmpHaddDir" || ! -d "\$tmpHaddDir" ]]; then
  echo "Temporary directory for hadd does not exist : \$tmpHaddDir"
  exit 1
fi

dirTmp=\$(echo $outputDir | sed "s#\$outputBase#\$tmpHaddDir#g")
mergedOutputName=\$(basename \$dirTmp)".root"
mergedOutDir=\$(dirname \$dirTmp)
mergedOutput=\$mergedOutDir"/"\$mergedOutputName
# if dirTmp is "my/merged/output/", then mergedOutput is "my/merged/output.root"

mkdir -p \$mergedOutDir
if [ \$nFiles -eq 1 ];
then
  # there is only one output file, no need to use hadd
  set -x
  cp \$jobOutput \$mergedOutput
  # also copy log files since there is only one job
  outputLog="\${mergedOutput/.root/.log}"
  cp $submitDir/logs/0.out \$outputLog
  outputErr="\${mergedOutput/.root/.err}"
  cp $submitDir/logs/0.err \$outputErr
  set +x
else
  hadd -f \$mergedOutput \$jobOutput
fi

EOF
chmod u+x $submitDir/mergeOutput.sh

# create a simple script to delete the directory with job outputs
cat > $submitDir/rmJobOutput.sh <<EOF
#!/bin/bash

rm -rf $outputDir

# $? is the exit status of last run command
if [ \$? -ne 0 ]; then
  if [ $isSubmitMIT -gt 0 ]; then
    srmPrefix="/mnt/hadoop/"
    outputDirTmp=$outputDir
    outputDirSRM=\${outputDirTmp#\${srmPrefix}}
    gfal-rm -r gsiftp://se01.cmsaf.mit.edu:2811/\${outputDirSRM}
  fi
fi

EOF
chmod u+x $submitDir/rmJobOutput.sh

condorSubmitOpt=""
if [ $isSubmitMIT -gt 0 ]; then
  condorSubmitOpt="-name submit.mit.edu"
fi

cat $submitDir/submit.condor
echo "condor_submit $submitDir/submit.condor $condorSubmitOpt"
