#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 -Wno-narrowing `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/drawSpectra.d Performance/drawSpectra.C -o Performance/drawSpectra.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawSpectra.exe"
inputList=(
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p10.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p15.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p20.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p30.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p40.list"
);

configFiles=(
"Configurations/photons/spectra2D.photon.SCnBC.conf"
"Configurations/photons/spectra2D.photon.SCnBC.GED.conf"
"Configurations/photons/spectra2D.photon.SCnBC.GED.conf"
"Configurations/photons/spectra2D.photon.SCnBC.GED.conf"
"Configurations/photons/spectra2D.photon.SCnBC.GED.conf"
"Configurations/photons/spectra2D.photon.SCnBC.GED.conf"
"Configurations/photons/spectra2D.photon.SCnBC.GED.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB"
outList=(
$outDirBase"/Configurations/photons/spectra2D_photon_SCnBC_"$outputSuffix"_extendEC.root"
$outDirBase"/Configurations/photons/spectra2D_photon_SCnBC_GED_"$outputSuffix"_extendEC.root"
$outDirBase"/Configurations/photons/spectra2D_photon_SCnBC_GED_"$outputSuffix"_extendEC_phiWidth0p10.root"
$outDirBase"/Configurations/photons/spectra2D_photon_SCnBC_GED_"$outputSuffix"_extendEC_phiWidth0p15.root"
$outDirBase"/Configurations/photons/spectra2D_photon_SCnBC_GED_"$outputSuffix"_extendEC_phiWidth0p20.root"
$outDirBase"/Configurations/photons/spectra2D_photon_SCnBC_GED_"$outputSuffix"_extendEC_phiWidth0p30.root"
$outDirBase"/Configurations/photons/spectra2D_photon_SCnBC_GED_"$outputSuffix"_extendEC_phiWidth0p40.root"
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


