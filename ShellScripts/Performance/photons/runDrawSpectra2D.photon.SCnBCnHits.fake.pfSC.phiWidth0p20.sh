#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 -Wno-narrowing `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/drawSpectra.d Performance/drawSpectra.C -o Performance/drawSpectra.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawSpectra.exe"
inputList=(
"Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC.list"
"Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC.list"
"Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p20.list"
);

configFiles=(
"Configurations/photons/spectra2D.photon.SCnBCnHits.fake.conf"
"Configurations/photons/spectra2D.photon.SCnBCnHits.fake.GED.conf"
"Configurations/photons/spectra2D.photon.SCnBCnHits.fake.GED.conf"
);

outDirBase="/export/d00/scratch/"$USER"/EWJTA-out/"
outputSuffix="Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB"
outputFiles=(
$outDirBase"Configurations/photons/spectra2D_photon_SCnBCnHits_fake_"$outputSuffix"_extendEC.root"
$outDirBase"Configurations/photons/spectra2D_photon_SCnBCnHits_fake_GED_"$outputSuffix"_extendEC.root"
$outDirBase"Configurations/photons/spectra2D_photon_SCnBCnHits_fake_GED_"$outputSuffix"_extendEC_phiWidth0p20.root"
);

arrayIndices=${!outputFiles[*]}
for i1 in $arrayIndices
do
    configFile=${configFiles[i1]}
    inputFile=${inputList[i1]}
    outputFile=${outputFiles[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done

