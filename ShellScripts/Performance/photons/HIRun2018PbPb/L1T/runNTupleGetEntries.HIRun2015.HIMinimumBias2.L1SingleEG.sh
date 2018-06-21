#!/bin/bash
# Estimate the L1 EG rates in data.

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/nTupleGetEntries.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputFileTmp="Configurations/filelists/HIMinimumBias2_HIRun2015-PromptReco-v1-Run263233-263284_FOREST_2345.list"
## create the file list during execution
inputDirTmp="/mnt/hadoop/cms/store/user/tatar/HIMinimumBias2/HIRun2015-PromptReco-v1-Run263233-263284-FOREST"
rm -f $inputFileTmp
ls -1 $inputDirTmp"/2.root" >> $inputFileTmp
ls -1 $inputDirTmp"/3.root" >> $inputFileTmp
ls -1 $inputDirTmp"/4.root" >> $inputFileTmp
ls -1 $inputDirTmp"/5.root" >> $inputFileTmp

inputList=(
$inputFileTmp
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="HIMinimumBias2_HIRun2015_PromptReco_v1_Run263233_263284"
outList=(
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$outputSuffix"_L1SingleEG.log"
);

argFileTmp="./Performance/nTupleGetEntries.args.txt"
rm -f $argFileTmp
echo "==selections==" >> $argFileTmp
echo "L1_SingleEG3_BptxAND > 0" >> $argFileTmp
echo "L1_SingleEG7_BptxAND > 0" >> $argFileTmp
echo "L1_SingleEG15_BptxAND > 0" >> $argFileTmp
echo "L1_SingleEG21_BptxAND > 0" >> $argFileTmp
echo "L1_SingleEG30_BptxAND > 0" >> $argFileTmp
echo "==trees==" >> $argFileTmp
echo "hltanalysis/HltTree" >> $argFileTmp

argFiles=(
$argFileTmp
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    inputFile=${inputList[i1]}
    outputFile=${outList[i1]}
    argFile=${argFiles[i1]}
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $inputFile $argFile &> $outputFile &
    echo "$runCmd $progPath $inputFile $argFile &> $outputFile &"
done


