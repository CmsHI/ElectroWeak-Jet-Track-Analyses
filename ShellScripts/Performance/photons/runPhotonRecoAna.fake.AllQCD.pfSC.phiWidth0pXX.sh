#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/photons/photonRecoAna.d Performance/photons/photonRecoAna.C -o Performance/photons/photonRecoAna.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonRecoAna.exe"
inputList=(
"Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC.list"
"Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC.list"
"Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC.list"
"Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC.list"
"Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p20.list"
"Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p20.list"
"Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p30.list"
"Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p30.list"
"Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p40.list"
"Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p40.list"
);

outDir="/export/d00/scratch/"$USER"/EWJTA-out/Performance/photons/"
outList=(
$outDir"photonRecoAna_fake_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC.root"
$outDir"photonRecoAna_fake_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC.root"
$outDir"photonRecoAna_fake_SIG_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC.root"
$outDir"photonRecoAna_fake_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC.root"
$outDir"photonRecoAna_fake_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p20.root"
$outDir"photonRecoAna_fake_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p20.root"
$outDir"photonRecoAna_fake_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p30.root"
$outDir"photonRecoAna_fake_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p30.root"
$outDir"photonRecoAna_fake_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p40.root"
$outDir"photonRecoAna_fake_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p40.root"
);

configList=(
"Configurations/Performance/photons/photonRecoAna.fake.AllQCD.conf"
"Configurations/Performance/photons/photonRecoAna.fake.AllQCD.GED.conf"
"Configurations/Performance/photons/photonRecoAna.fake.AllQCD.SIG.conf"
"Configurations/Performance/photons/photonRecoAna.fake.AllQCD.SIG.GED.conf"
"Configurations/Performance/photons/photonRecoAna.fake.AllQCD.GED.conf"
"Configurations/Performance/photons/photonRecoAna.fake.AllQCD.SIG.GED.conf"
"Configurations/Performance/photons/photonRecoAna.fake.AllQCD.GED.conf"
"Configurations/Performance/photons/photonRecoAna.fake.AllQCD.SIG.GED.conf"
"Configurations/Performance/photons/photonRecoAna.fake.AllQCD.GED.conf"
"Configurations/Performance/photons/photonRecoAna.fake.AllQCD.SIG.GED.conf"
);

mkdir -p outDir

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    inputFile=${inputList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done
