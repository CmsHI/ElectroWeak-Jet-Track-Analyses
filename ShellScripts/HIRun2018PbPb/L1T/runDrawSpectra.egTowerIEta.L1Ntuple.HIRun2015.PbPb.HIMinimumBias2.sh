#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 -Wno-narrowing `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/drawSpectra.d Performance/drawSpectra.C -o Performance/drawSpectra.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawSpectra.exe"
inputList=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassEGVetos1/180318_055100/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1ExtHOverE1_egHOverEcut_EB1_EE1_BypassPt30/180327_034514/L1Ntuple_merged.root"
);

configFiles=(
"Configurations/HIRun2018PbPb/L1T/spectra.egTowerIEta.conf"
"Configurations/HIRun2018PbPb/L1T/spectra.egTowerIEta.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="L1Ntuple_HIRun2015_HIMinimumBias2"
outList=(
$outDirBase"/Configurations/HIRun2018PbPb/L1T/spectra_egTowerIEta_"$outputSuffix"_egBypassEGVetos1.root"
$outDirBase"/Configurations/HIRun2018PbPb/L1T/spectra_egTowerIEta_"$outputSuffix"_egBypassFGBit1ShapeBit1ExtHOverE1_egHOverEcut_EB1_EE1_BypassPt30.root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configFiles[i1]}
    inputFile=${inputList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done

