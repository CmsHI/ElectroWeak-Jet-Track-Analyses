#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Plotting/plotHistogram.exe"

baseDir="/directory/of/configFiles/"

configFiles=(
$baseDir"plot.pt.conf"
$baseDir"plot.eta.conf"
$baseDir"plot.phi.conf"
);

arrayIndices=${!configFiles[*]}
for i1 in $arrayIndices
do
    configFile=${configFiles[i1]}
    inputFile=$configFile  # assume input files are listed in the config file
    outputFile="${configFile/.conf/.png}"
    outputFileLOG="${configFile/.conf/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done
