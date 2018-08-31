#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/nTupleGetEntries.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/hltTestEgamma_V29/openHLT_merged.root"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018_hltTestEgamma_V29"
outList=(
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$outputSuffix".log"
);

argFileTmp="./Performance/nTupleGetEntries.args.txt"
rm -f $argFileTmp
echo "==selections==" >> $argFileTmp
echo "HLT_GEDPhoton10 > 0" >> $argFileTmp
echo "HLT_GEDPhoton15 > 0" >> $argFileTmp
echo "HLT_GEDPhoton20 > 0" >> $argFileTmp
echo "HLT_GEDPhoton30 > 0" >> $argFileTmp
echo "HLT_GEDPhoton40 > 0" >> $argFileTmp
echo "HLT_GEDPhoton50 > 0" >> $argFileTmp
echo "HLT_GEDPhoton60 > 0" >> $argFileTmp
echo "HLT_GEDPhoton10_EB > 0" >> $argFileTmp
echo "HLT_GEDPhoton15_EB > 0" >> $argFileTmp
echo "HLT_GEDPhoton20_EB > 0" >> $argFileTmp
echo "HLT_GEDPhoton30_EB > 0" >> $argFileTmp
echo "HLT_GEDPhoton40_EB > 0" >> $argFileTmp
echo "HLT_GEDPhoton50_EB > 0" >> $argFileTmp
echo "HLT_GEDPhoton60_EB > 0" >> $argFileTmp
echo "HLT_HISinglePhoton10_Eta3p1 > 0" >> $argFileTmp
echo "HLT_HISinglePhoton15_Eta3p1 > 0" >> $argFileTmp
echo "HLT_HISinglePhoton20_Eta3p1 > 0" >> $argFileTmp
echo "HLT_HISinglePhoton30_Eta3p1 > 0" >> $argFileTmp
echo "HLT_HISinglePhoton40_Eta3p1 > 0" >> $argFileTmp
echo "HLT_HISinglePhoton50_Eta3p1 > 0" >> $argFileTmp
echo "HLT_HISinglePhoton60_Eta3p1 > 0" >> $argFileTmp
echo "HLT_HISinglePhoton10_Eta1p5 > 0" >> $argFileTmp
echo "HLT_HISinglePhoton15_Eta1p5 > 0" >> $argFileTmp
echo "HLT_HISinglePhoton20_Eta1p5 > 0" >> $argFileTmp
echo "HLT_HISinglePhoton30_Eta1p5 > 0" >> $argFileTmp
echo "HLT_HISinglePhoton40_Eta1p5 > 0" >> $argFileTmp
echo "HLT_HISinglePhoton50_Eta1p5 > 0" >> $argFileTmp
echo "HLT_HISinglePhoton60_Eta1p5 > 0" >> $argFileTmp
echo "==trees==" >> $argFileTmp
echo "hltbitanalysis/HltTree" >> $argFileTmp

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


