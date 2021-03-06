#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 -Wno-narrowing `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/drawSpectra.d Performance/drawSpectra.C -o Performance/drawSpectra.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawSpectra.exe"
inputFile="Configurations/filelists/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias.list"
#inputFile="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1Trigger/L1Ntuple_XeXeRun2017_HIMinimumBias/180123_060142/L1Ntuple_merged.root"

configFiles=(
"Configurations/HIRun2018PbPb/L1T/spectra.L1CaloTower.iet.conf"
"Configurations/HIRun2018PbPb/L1T/spectra.L1CaloTower.ieta.iphi.conf"
"Configurations/HIRun2018PbPb/L1T/spectra2D.L1CaloTower.ieta.iphi.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="L1Ntuple_XeXeRun2017_HIMinimumBias"
outList=(
$outDirBase"/Configurations/HIRun2018PbPb/L1T/spectra_L1CaloTower_iet_"$outputSuffix".root"
$outDirBase"/Configurations/HIRun2018PbPb/L1T/spectra_L1CaloTower_ieta_iphi_"$outputSuffix".root"
$outDirBase"/Configurations/HIRun2018PbPb/L1T/spectra2D_L1CaloTower_ieta_iphi_"$outputSuffix".root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configFiles[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done
