#!/bin/bash

# corresponding commit for Plotting/plotHistogram.C
# commit a5a56c610ab8ce76833f132efab4f4cf1970c218
# https://github.com/CmsHI/ElectroWeak-Jet-Track-Analyses/blob/a5a56c610ab8ce76833f132efab4f4cf1970c218/Plotting/plotHistogram.C
runCmd="./ShellScripts/myRun.sh"
progPath="./Plotting/plotHistogram.exe"

baseDir="Configurations/photonJetJS/event_fraction/"

configFiles=(
"plot.phopt.processID.fraction.Pythia8.AllQCDPhoton.HINppWinter16DR.conf"
"plot.phopt.processID.mcCalIso.fraction.Pythia8.AllQCDPhoton.HINppWinter16DR.conf"
);

arrayIndices=${!configFiles[*]}
for i1 in $arrayIndices
do
    configFile=$baseDir${configFiles[i1]}
    inputFile=$configFile  # assume input files are listed in the config file
    outputFile="${configFile/.conf/.pdf}"
    outputFileLOG="${configFile/.conf/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG"
done
