#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/nTupleGetEntries.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/hltTestEgamma_V32/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/hltTestEgamma_V32_loose_hltRechitInRegionsECAL/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/hltTestEgamma_V32_hltEcalRecHit/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/HIMinimumBias8_XeXeRun2017/hltTestEgamma_V32/openHLT_merged.root"
## V34
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/hltTestEgamma_V34/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/HIMinimumBias8_XeXeRun2017/hltTestEgamma_V34/openHLT_merged.root"
#"Configurations/filelists/HIRun2018PbPb/HLT/HIMinimumBias8_XeXeRun2017_hltTestEgamma_V34_openHLT.list"
## copy_katatar_hltPbPb2018Photons
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/copy_hltPbPb2018Photons_V2/job0.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/copy_hltPbPb2018Photons_V3/job0.root"
## hltPbPb2018Photons
"Configurations/filelists/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018_hltPbPb2018Photons_V10_openHLT.list"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/hltPbPb2018Photons_V11/job0.root"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
#outputSuffix="Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018_hltTestEgamma_V34"
outputSuffix="Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018"
outList=(
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$outputSuffix".log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$outputSuffix"_loose_hltRechitInRegionsECAL.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$outputSuffix"_hltEcalRecHit.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HIMinimumBias8_XeXeRun2017_hltTestEgamma_V32.log"
## V34
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$outputSuffix".log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HIMinimumBias8_XeXeRun2017_hltTestEgamma_V34.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HIMinimumBias8_XeXeRun2017_hltTestEgamma_V34_v2.log"
## copy_katatar_hltPbPb2018Photons
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$outputSuffix"_copy_hltPbPb2018Photons_V2.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$outputSuffix"_copy_hltPbPb2018Photons_V3.log"
## hltPbPb2018Photons
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$outputSuffix"_hltPbPb2018Photons_V10.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$outputSuffix"_hltPbPb2018Photons_V11.log"
);

argFileTmp="./Performance/nTupleGetEntries.args.txt"
rm -f $argFileTmp
echo "==selections==" >> $argFileTmp
echo "HLT_HIGEDPhoton10_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton15_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton20_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton30_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton40_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton50_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton60_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton10_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton15_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton20_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton30_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton40_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton50_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton60_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton10_EB_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton15_EB_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton20_EB_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton30_EB_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton40_EB_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton50_EB_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton60_EB_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton10_EB_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton15_EB_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton20_EB_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton30_EB_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton40_EB_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton50_EB_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton60_EB_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton10_Eta3p1_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton15_Eta3p1_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton20_Eta3p1_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton30_Eta3p1_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton40_Eta3p1_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton50_Eta3p1_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton60_Eta3p1_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton10_Eta1p5_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton15_Eta1p5_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton20_Eta1p5_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton30_Eta1p5_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton40_Eta1p5_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton50_Eta1p5_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton60_Eta1p5_v1 > 0" >> $argFileTmp
echo "==trees==" >> $argFileTmp
echo "hltbitanalysis/HltTree" >> $argFileTmp

argFiles=(
#$argFileTmp
#$argFileTmp
#$argFileTmp
#$argFileTmp
## V34
#$argFileTmp
#$argFileTmp
#$argFileTmp
## copy_katatar_hltPbPb2018Photons
#$argFileTmp
#$argFileTmp
## hltPbPb2018Photons
$argFileTmp
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


