#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/hiForestPickSkim.exe"
inputFileTmp="Configurations/filelists/Pythia8_AllQCDPhoton30Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list"
inputFile="${inputFileTmp/.list/_TMP.list}"

cat $inputFileTmp | head -10 > $inputFile

configFiles=(
"Configurations/examples/hiForestPickSkim.conf"
);

eventLists=(
"/export/d00/scratch/"$USER"/EWJTA-out/Configurations/examples/printRLE_Pythia8_AllQCDPhoton30Flt30_Hydjet_Cymbal_MB.txt"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="Pythia8_AllQCDPhoton30Flt30_Hydjet_Cymbal_MB"
outList=(
$outDirBase"/Configurations/examples/hiForestPickSkim_"$outputSuffix".root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configFiles[i1]}
    eventList=${eventLists[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $eventList $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $eventList $inputFile $outputFile &> $outputFileLOG &"
done


