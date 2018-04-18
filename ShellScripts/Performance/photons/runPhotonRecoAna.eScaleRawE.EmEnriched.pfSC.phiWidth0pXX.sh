#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/photons/photonRecoAna.d Performance/photons/photonRecoAna.C -o Performance/photons/photonRecoAna.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonRecoAna.exe"
inputList=(
"Configurations/filelists/photons/Pythia8_EmEnrichedDijet30_Hydjet_Cymbal_MB_FOREST_extendEC.list"
"Configurations/filelists/photons/Pythia8_EmEnrichedDijet30_Hydjet_Cymbal_MB_FOREST_extendEC.list"
"Configurations/filelists/photons/Pythia8_EmEnrichedDijet30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p15.list"
"Configurations/filelists/photons/Pythia8_EmEnrichedDijet30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p20.list"
"Configurations/filelists/photons/Pythia8_EmEnrichedDijet30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p30.list"
);

configList=(
"Configurations/Performance/photons/photonRecoAna.eScaleRawE.EmEnriched.conf"
"Configurations/Performance/photons/photonRecoAna.eScaleRawE.EmEnriched.GED.conf"
"Configurations/Performance/photons/photonRecoAna.eScaleRawE.EmEnriched.GED.conf"
"Configurations/Performance/photons/photonRecoAna.eScaleRawE.EmEnriched.GED.conf"
"Configurations/Performance/photons/photonRecoAna.eScaleRawE.EmEnriched.GED.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="EmEnrichedDijet30_Hydjet_Cymbal_MB_FOREST_extendEC"
outList=(
$outDirBase"/Performance/photons/photonRecoAna_eScaleRawE_"$outputSuffix".root"
$outDirBase"/Performance/photons/photonRecoAna_eScaleRawE_GED_"$outputSuffix".root"
$outDirBase"/Performance/photons/photonRecoAna_eScaleRawE_GED_"$outputSuffix"_pfSC_phiWidth0p15.root"
$outDirBase"/Performance/photons/photonRecoAna_eScaleRawE_GED_"$outputSuffix"_pfSC_phiWidth0p20.root"
$outDirBase"/Performance/photons/photonRecoAna_eScaleRawE_GED_"$outputSuffix"_pfSC_phiWidth0p30.root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    inputFile=${inputList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done

