#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/nTupleGetEntries.exe"
inputFile="Configurations/filelists/Pythia8_Photon30_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"

#make $progPath || exit 1

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="Pythia8_Photon30_pp502_TuneCUETP8M1_HINppWinter16DR"
outList=(
$outDirBase"/Performance/nTupleGetEntries_"$outputSuffix".log"
);

argFileTmp="./Performance/nTupleGetEntries.args.txt"
rm -f $argFileTmp
echo "==selections==" >> $argFileTmp
echo "phoEt > 80" >> $argFileTmp
echo "phoEt > 80 && abs(phoEta) < 1.44" >> $argFileTmp
echo "phoEt > 60 && abs(phoEta) < 1.44 && jtpt > 30" >> $argFileTmp
echo "==trees==" >> $argFileTmp
echo "ggHiNtuplizerGED/EventTree" >> $argFileTmp
echo "ak3PFJetAnalyzer/t" >> $argFileTmp

argFiles=(
$argFileTmp
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    outputFile=${outList[i1]}
    argFile=${argFiles[i1]}
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $inputFile $argFile &> $outputFile &
    echo "$runCmd $progPath $inputFile $argFile &> $outputFile &"
done


