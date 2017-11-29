#!/bin/bash
# creates a script in a given directory and that script runs plotHistogram.C on all configuration files in that directory

if [ $# -ne 1 ]; then
  echo "Usage: ./ShellScripts/Plotting/createRunPlots.sh <pathToDirectory>"
  echo "Example: ./ShellScripts/Plotting/createRunPlots.sh ./plotsDir/"
  exit 1
fi


outputTmp=$1"/dummy" # append dummy string so that directory path can be created with exactly one trailing "/"
outputDir=$(dirname "${outputTmp}")"/"
echo "outputDir = $outputDir"
if [ ! -d $outputDir ]; then
   echo "Directory $outputDir does not exist. Exiting."
   exit 1
fi

outputFile=$outputDir"runPlots.sh"   
if [ -f $outputFile ]; then
   echo "Script $outputFile exists already. Exiting."
   exit 1
fi

cat > $outputFile <<EOF
#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Plotting/plotHistogram.exe"

baseDir="$outputDir"

configFiles=(
EOF

echo "list of config files to be put into shell script"
confFilesWC=$outputDir"*.conf"
for confFile in $confFilesWC
do
   confFileBase=$(basename "${confFile}")
   echo $confFileBase
   # write $confFileBase to the new script
   echo -e "\$baseDir""\"$confFileBase\"" >> $outputFile
done

# continue editing the file
cat >> $outputFile <<EOF
);

arrayIndices=\${!configFiles[*]}
for i1 in \$arrayIndices
do
    configFile=\${configFiles[i1]}
    inputFile=\$configFile  # assume input files are listed in the config file
    outputFile="\${configFile/.conf/.png}"
    outputFileLOG="\${configFile/.conf/.log}"
    outDir=\$(dirname "\${outputFile}")
    mkdir -p \$outDir
    \$runCmd \$progPath \$configFile \$inputFile \$outputFile &> \$outputFileLOG &
    echo "\$runCmd \$progPath \$configFile \$inputFile \$outputFile &> \$outputFileLOG &"
done
EOF

chmod u+x $outputFile
echo "Script has been created at $outputFile"

