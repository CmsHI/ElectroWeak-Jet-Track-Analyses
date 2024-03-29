#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/nTupleGetEntries.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
## hltTestEgamma
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/hltTestEgamma_V32/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/HIMinimumBias8_XeXeRun2017/hltTestEgamma_V32/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/hltTestEgamma_V68/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V68/openHLT_merged.root"
## 103X_hltTestEgamma_V68
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8/103X_hltTestEgamma_V68/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8/103X_hltTestEgamma_V68_L1_SK1212/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8/103X_hltTestEgamma_V68_L1_SK1207/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8/103X_hltTestEgamma_V68_L1_SK1616/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_hltTestEgamma_V68/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_hltTestEgamma_V68_L1_SK1212/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_hltTestEgamma_V68_L1_SK1207/openHLT_merged.root"
## 103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_noReEmul/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_L1_defaultSK/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_L1_SK1212/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_L1_SK1207/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_L1_SK1207_egEtaCut26/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_L1_SK1207_egEtaCut28/openHLT_merged.root"
## 103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V72
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V72_L1_defaultSK/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V72_L1_SK1212/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V72_L1_SK1207/openHLT_merged.root"
## 103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V73
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V73_L1_SK1207/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton15_bias_Hydjet_Drum5Ev8_5020GeV/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V73_L1_SK1207/openHLT_merged.root"
## hltPbPb2018Photons
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/hltPbPb2018Photons_V10/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/hltPbPb2018Photons_V11/openHLT_merged.root"
#"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/hltPbPb2018Photons_V18/openHLT_merged.root"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
suffixAllQCDPhoton15="Pythia8_AllQCDPhoton15_bias_Hydjet_Drum5Ev8_5020GeV"
suffixHydjet="Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018"
outList=(
## hltTestEgamma
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$suffixHydjet"_hltTestEgamma_V32.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HIMinimumBias8_XeXeRun2017_hltTestEgamma_V32.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$suffixHydjet"_hltTestEgamma_V68.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$suffixAllQCDPhoton15"_hltTestEgamma_V68.log"
## 103X_hltTestEgamma_V68
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixAllQCDPhoton30"_103X_hltTestEgamma_V68.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixAllQCDPhoton30"_103X_hltTestEgamma_V68_L1_SK1212.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixAllQCDPhoton30"_103X_hltTestEgamma_V68_L1_SK1207.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixAllQCDPhoton30"_103X_hltTestEgamma_V68_L1_SK1616.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixHydjet"_103X_hltTestEgamma_V68.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixHydjet"_103X_hltTestEgamma_V68_L1_SK1212.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixHydjet"_103X_hltTestEgamma_V68_L1_SK1207.log"
## 103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixHydjet"_103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_noReEmul.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixHydjet"_103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_L1_defaultSK.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixHydjet"_103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_L1_SK1212.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixHydjet"_103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_L1_SK1207.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixHydjet"_103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_L1_SK1207_egEtaCut26.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixHydjet"_103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_L1_SK1207_egEtaCut28.log"
## 103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V72
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixHydjet"_103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V72_L1_defaultSK.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixHydjet"_103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V72_L1_SK1212.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixHydjet"_103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V72_L1_SK1207.log"
## 103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V73
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixHydjet"_103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V73_L1_SK1207.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_HLT_"$suffixAllQCDPhoton15"_103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V73_L1_SK1207.log"
## hltPbPb2018Photons
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$suffixHydjet"_hltPbPb2018Photons_V10.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$suffixHydjet"_hltPbPb2018Photons_V11.log"
#$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/nTupleGetEntries_"$suffixAllQCDPhoton15"_hltPbPb2018Photons_V18.log"
);

argFileTmp="./Performance/nTupleGetEntries.args.txt"
rm -f $argFileTmp
echo "==selections==" >> $argFileTmp
echo "HLT_HIGEDPhoton10_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton20_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton30_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton40_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton50_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton60_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton10_EB_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton20_EB_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton30_EB_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton40_EB_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton50_EB_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton60_EB_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton10_Eta2p4_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton20_Eta2p4_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton30_Eta2p4_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton40_Eta2p4_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton50_Eta2p4_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton60_Eta2p4_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton10_Eta1p5_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton20_Eta1p5_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton30_Eta1p5_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton40_Eta1p5_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton50_Eta1p5_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton60_Eta1p5_v1 > 0" >> $argFileTmp
## Cent30_100
echo "HLT_HIGEDPhoton10_Cent30_100_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton20_Cent30_100_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton30_Cent30_100_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton40_Cent30_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton10_Eta2p4_Cent30_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton20_Eta2p4_Cent30_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton30_Eta2p4_Cent30_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton40_Eta2p4_Cent30_100_v1 > 0" >> $argFileTmp
## Cent50_100
echo "HLT_HIGEDPhoton10_Cent50_100_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton20_Cent50_100_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton30_Cent50_100_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton40_Cent50_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton10_Eta2p4_Cent50_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton20_Eta2p4_Cent50_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton30_Eta2p4_Cent50_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton40_Eta2p4_Cent50_100_v1 > 0" >> $argFileTmp
## Island Eta3p1
echo "HLT_HIIslandPhoton10_Eta3p1_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton20_Eta3p1_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton30_Eta3p1_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton40_Eta3p1_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton50_Eta3p1_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton60_Eta3p1_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton10_Eta3p1_Cent30_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton20_Eta3p1_Cent30_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton30_Eta3p1_Cent30_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton40_Eta3p1_Cent30_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton10_Eta3p1_Cent50_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton20_Eta3p1_Cent50_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton30_Eta3p1_Cent50_100_v1 > 0" >> $argFileTmp
echo "HLT_HIIslandPhoton40_Eta3p1_Cent50_100_v1 > 0" >> $argFileTmp
## L1EG3
echo "HLT_HIGEDPhoton10_L1EG3_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton20_L1EG3_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton10_EB_L1EG3_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton20_EB_L1EG3_v1 > 0" >> $argFileTmp
## L1Seeded
echo "HLT_HIGEDPhoton10_L1Seeded_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton20_L1Seeded_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton30_L1Seeded_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton40_L1Seeded_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton50_L1Seeded_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton60_L1Seeded_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton10_EB_L1Seeded_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton20_EB_L1Seeded_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton30_EB_L1Seeded_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton40_EB_L1Seeded_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton50_EB_L1Seeded_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton60_EB_L1Seeded_v1 > 0" >> $argFileTmp
## HECut
echo "HLT_HIGEDPhoton10_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton20_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton30_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton40_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton50_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton60_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton10_EB_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton20_EB_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton30_EB_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton40_EB_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton50_EB_HECut_v1 > 0" >> $argFileTmp
echo "HLT_HIGEDPhoton60_EB_HECut_v1 > 0" >> $argFileTmp
echo "==trees==" >> $argFileTmp
echo "hltbitanalysis/HltTree" >> $argFileTmp

argFiles=(
## hltTestEgamma
#$argFileTmp
#$argFileTmp
#$argFileTmp
#$argFileTmp
## 103X_hltTestEgamma_V68
#$argFileTmp
#$argFileTmp
#$argFileTmp
#$argFileTmp
#$argFileTmp
#$argFileTmp
#$argFileTmp
## 103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70
#$argFileTmp
#$argFileTmp
#$argFileTmp
#$argFileTmp
#$argFileTmp
#$argFileTmp
## 103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V72
$argFileTmp
$argFileTmp
$argFileTmp
## 103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V73
$argFileTmp
$argFileTmp
## hltPbPb2018Photons
#$argFileTmp
#$argFileTmp
#$argFileTmp
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


