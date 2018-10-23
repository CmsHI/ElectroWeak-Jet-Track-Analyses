#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/nTupleGetEntries.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
## hltTestEgamma
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V64/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V64_L1_SK1212/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V64_L1_SK1207/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/hltTestEgamma_V68/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_hltTestEgamma_V68/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_hltTestEgamma_V68_L1_SK1212/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/103X_hltTestEgamma_V68_L1_SK1207/openHLT_merged.root"
## hltPbPb2018Photons
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/hltPbPb2018Photons_V11/openHLT_merged.root"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
suffixAllQCDPhoton="Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8"
suffixHydjet="Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018"
outList=(
## hltTestEgamma
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_L1T_"$suffixAllQCDPhoton"_hltTestEgamma_V64.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_L1T_"$suffixAllQCDPhoton"_hltTestEgamma_V64_L1_SK1212.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_L1T_"$suffixAllQCDPhoton"_hltTestEgamma_V64_L1_SK1207.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_L1T_"$suffixHydjet"_hltTestEgamma_V68.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_L1T_"$suffixHydjet"_103X_hltTestEgamma_V68.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_L1T_"$suffixHydjet"_103X_hltTestEgamma_V68_L1_SK1212.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_L1T_"$suffixHydjet"_103X_hltTestEgamma_V68_L1_SK1207.log"
## hltPbPb2018Photons
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_L1T_"$suffixHydjet"_hltPbPb2018Photons_V11.log"
);

argFileTmp="./Performance/nTupleGetEntries.args.txt"
rm -f $argFileTmp
echo "==selections==" >> $argFileTmp
## ZeroBias
echo "L1_ZeroBias > 0" >> $argFileTmp
echo "L1_ZeroBias_copy > 0" >> $argFileTmp
## MinimumBias
echo "L1_MinimumBiasHF1_AND > 0" >> $argFileTmp
echo "L1_MinimumBiasHF2_AND > 0" >> $argFileTmp
echo "L1_MinimumBiasHF1_OR > 0" >> $argFileTmp
echo "L1_MinimumBiasHF2_OR > 0" >> $argFileTmp
echo "L1_MinimumBiasHF1_OR_BptxAND > 0" >> $argFileTmp
echo "L1_MinimumBiasHF2_OR_BptxAND > 0" >> $argFileTmp
echo "L1_MinimumBiasHF1_AND_BptxAND > 0" >> $argFileTmp
echo "L1_MinimumBiasHF2_AND_BptxAND > 0" >> $argFileTmp
echo "L1_MinimumBiasHF1_XOR_BptxAND > 0" >> $argFileTmp
## Centrality
echo "L1_Centrality_20_100_MinimumBiasHF1_AND_BptxAND" >> $argFileTmp
echo "L1_Centrality_30_100_MinimumBiasHF1_AND_BptxAND" >> $argFileTmp
echo "L1_Centrality_50_100" >> $argFileTmp
echo "L1_Centrality_30_100" >> $argFileTmp
## SingleEG
echo "L1_SingleEG3_BptxAND" >> $argFileTmp
echo "L1_SingleEG5_BptxAND" >> $argFileTmp
echo "L1_SingleEG7_BptxAND" >> $argFileTmp
echo "L1_SingleEG12_BptxAND" >> $argFileTmp
echo "L1_SingleEG15_BptxAND" >> $argFileTmp
echo "L1_SingleEG21_BptxAND" >> $argFileTmp
echo "L1_SingleEG30_BptxAND" >> $argFileTmp
echo "L1_SingleEG3_Centrality_30_100_BptxAND" >> $argFileTmp
echo "L1_SingleEG7_Centrality_30_100_BptxAND" >> $argFileTmp
echo "L1_SingleEG15_Centrality_30_100_BptxAND" >> $argFileTmp
echo "L1_SingleEG21_Centrality_30_100_BptxAND" >> $argFileTmp
echo "L1_SingleEG3_Centrality_50_100_BptxAND" >> $argFileTmp
echo "L1_SingleEG7_Centrality_50_100_BptxAND" >> $argFileTmp
echo "L1_SingleEG15_Centrality_50_100_BptxAND" >> $argFileTmp
echo "L1_SingleEG21_Centrality_50_100_BptxAND" >> $argFileTmp
echo "L1_SingleEG3" >> $argFileTmp
echo "L1_SingleEG5" >> $argFileTmp
echo "L1_SingleEG3_NotMinimumBiasHF2_OR" >> $argFileTmp
echo "L1_SingleEG5_NotMinimumBiasHF2_OR" >> $argFileTmp
echo "L1_SingleEG3_NotMinimumBiasHF2_AND" >> $argFileTmp
echo "L1_SingleEG5_NotMinimumBiasHF2_AND" >> $argFileTmp
echo "L1_SingleEG5_SingleJet28_MidEta2p7" >> $argFileTmp
echo "L1_SingleEG5_SingleJet32_MidEta2p7" >> $argFileTmp
echo "L1_SingleEG5_SingleJet40_MidEta2p7" >> $argFileTmp
echo "L1_SingleEG7_SingleJet32_MidEta2p7" >> $argFileTmp
echo "L1_SingleEG7_SingleJet40_MidEta2p7" >> $argFileTmp
echo "L1_SingleEG12_SingleJet40_MidEta2p7" >> $argFileTmp
echo "L1_SingleEG12_SingleJet32_MidEta2p7" >> $argFileTmp
## SingleIsoEG
echo "L1_SingleIsoEG3_BptxAND" >> $argFileTmp
echo "L1_SingleIsoEG7_BptxAND" >> $argFileTmp
echo "L1_SingleIsoEG12_BptxAND" >> $argFileTmp
echo "L1_SingleIsoEG15_BptxAND" >> $argFileTmp
echo "L1_SingleIsoEG21_BptxAND" >> $argFileTmp
echo "==trees==" >> $argFileTmp
echo "hltbitanalysis/HltTree" >> $argFileTmp

argFiles=(
## hltTestEgamma
$argFileTmp
$argFileTmp
$argFileTmp
$argFileTmp
$argFileTmp
$argFileTmp
$argFileTmp
## hltPbPb2018Photons
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


