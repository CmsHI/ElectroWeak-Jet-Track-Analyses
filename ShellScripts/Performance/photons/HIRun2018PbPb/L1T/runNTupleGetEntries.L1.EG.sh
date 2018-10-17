#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/nTupleGetEntries.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
## Hydjet
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24_SK1212/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24_SK1207/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24_SK0908/L1Ntuple_merged.root"
## AllQCDPhoton
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24_SK1212/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24_SK0908/L1Ntuple_merged.root"
## XeXe 2017
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias8_egHOverEcut_EB1_EE1_egEtaCut24/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias8_egHOverEcut_EB1_EE1_egEtaCut24_SK1616/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias8_egHOverEcut_EB1_EE1_egEtaCut24_SK1212/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias8_egHOverEcut_EB1_EE1_egEtaCut24_SK1207/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias8_egHOverEcut_EB1_EE1_egEtaCut24_SK0908/L1Ntuple_merged.root"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
suffixHydjet="Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018_L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24"
suffixAllQCDPhoton="Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8_L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24"
suffixXeXe2017="L1Ntuple_XeXeRun2017_HIMinimumBias8_egHOverEcut_EB1_EE1_egEtaCut24"
outList=(
## Hydjet
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$suffixHydjet".log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$suffixHydjet"_SK1212.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$suffixHydjet"_SK1207.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$suffixHydjet"_SK0908.log"
## AllQCDPhoton
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$suffixAllQCDPhoton".log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$suffixAllQCDPhoton"_SK1212.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$suffixAllQCDPhoton"_SK0908.log"
## XeXe 2017
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$suffixXeXe2017".log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$suffixXeXe2017"_SK1616.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$suffixXeXe2017"_SK1212.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$suffixXeXe2017"_SK1207.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$suffixXeXe2017"_SK0908.log"
);

argFileTmp="./Performance/nTupleGetEntries.args.txt"
rm -f $argFileTmp
echo "==selections==" >> $argFileTmp
echo "egEt > 3" >> $argFileTmp
echo "egEt > 5" >> $argFileTmp
echo "egEt > 7" >> $argFileTmp
echo "egEt > 12" >> $argFileTmp
echo "egEt > 15" >> $argFileTmp
echo "egEt > 21" >> $argFileTmp
echo "egEt > 30" >> $argFileTmp
echo "egEt > 3 && abs(egEta) < 2.4" >> $argFileTmp
echo "egEt > 5 && abs(egEta) < 2.4" >> $argFileTmp
echo "egEt > 7 && abs(egEta) < 2.4" >> $argFileTmp
echo "egEt > 12 && abs(egEta) < 2.4" >> $argFileTmp
echo "egEt > 15 && abs(egEta) < 2.4" >> $argFileTmp
echo "egEt > 21 && abs(egEta) < 2.4" >> $argFileTmp
echo "egEt > 30 && abs(egEta) < 2.4" >> $argFileTmp
echo "egEt > 3 && abs(egEta) < 1.48" >> $argFileTmp
echo "egEt > 5 && abs(egEta) < 1.48" >> $argFileTmp
echo "egEt > 7 && abs(egEta) < 1.48" >> $argFileTmp
echo "egEt > 12 && abs(egEta) < 1.48" >> $argFileTmp
echo "egEt > 15 && abs(egEta) < 1.48" >> $argFileTmp
echo "egEt > 21 && abs(egEta) < 1.48" >> $argFileTmp
echo "egEt > 30 && abs(egEta) < 1.48" >> $argFileTmp
echo "==trees==" >> $argFileTmp
echo "l1UpgradeEmuTree/L1UpgradeTree" >> $argFileTmp

argFiles=(
## Hydjet
$argFileTmp
$argFileTmp
$argFileTmp
$argFileTmp
## AllQCDPhoton
$argFileTmp
$argFileTmp
$argFileTmp
## XeXe 2017
$argFileTmp
$argFileTmp
$argFileTmp
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


