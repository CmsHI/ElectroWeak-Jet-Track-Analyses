#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/nTupleGetEntries.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassEGVetos1/180318_055100/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1_egBypassExtHOverE1_egHOverEcut_EB1_EE1/180322_231807/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1ExtHOverE1_egHOverEcut_EB1_EE1_BypassPt30/180327_034514/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1ExtHOverE1_egHOverEcut_EB1_EE1_BypassPt30_egEtaCut24/180420_210253/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1ExtHOverE1_egHOverEcut_EB1_EE1_BypassPt30_egEtaCut25/180420_210913/L1Ntuple_merged.root"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="HIMinimumBias2_HIRun2015_PromptReco_v1_Run263233_263284"
outList=(
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$outputSuffix"_L1T_egBypassEGVetos1.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$outputSuffix"_L1T_egBypassFGBit1ShapeBit1ExtHOverE1_egHOverEcut_EB1_EE1.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$outputSuffix"_L1T_egBypassFGBit1ShapeBit1ExtHOverE1_egHOverEcut_EB1_EE1_BypassPt30.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$outputSuffix"_L1T_egBypassFGBit1ShapeBit1ExtHOverE1_egHOverEcut_EB1_EE1_BypassPt30_egEtaCut24.log"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/nTupleGetEntries_"$outputSuffix"_L1T_egBypassFGBit1ShapeBit1ExtHOverE1_egHOverEcut_EB1_EE1_BypassPt30_egEtaCut25.log"
);

argFileTmp="./Performance/nTupleGetEntries.args.txt"
rm -f $argFileTmp
echo "==selections==" >> $argFileTmp
echo "egEt > 3 " >> $argFileTmp
echo "egEt > 7 " >> $argFileTmp
echo "egEt > 15" >> $argFileTmp
echo "egEt > 21" >> $argFileTmp
echo "egEt > 30" >> $argFileTmp
echo "egEt > 3  && abs(egEta) < 1.44" >> $argFileTmp
echo "egEt > 7  && abs(egEta) < 1.44" >> $argFileTmp
echo "egEt > 15 && abs(egEta) < 1.44" >> $argFileTmp
echo "egEt > 21 && abs(egEta) < 1.44" >> $argFileTmp
echo "egEt > 30 && abs(egEta) < 1.44" >> $argFileTmp
echo "egEt > 3  && abs(egEta) > 1.48 && abs(egEta) < 2.0" >> $argFileTmp
echo "egEt > 7  && abs(egEta) > 1.48 && abs(egEta) < 2.0" >> $argFileTmp
echo "egEt > 15 && abs(egEta) > 1.48 && abs(egEta) < 2.0" >> $argFileTmp
echo "egEt > 21 && abs(egEta) > 1.48 && abs(egEta) < 2.0" >> $argFileTmp
echo "egEt > 30 && abs(egEta) > 1.48 && abs(egEta) < 2.0" >> $argFileTmp
echo "==trees==" >> $argFileTmp
echo "l1UpgradeEmuTree/L1UpgradeTree" >> $argFileTmp

argFiles=(
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


