#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/photons/photonRecoAna.d Performance/photons/photonRecoAna.C -o Performance/photons/photonRecoAna.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonRecoAna.exe"
inputList=(
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p10.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p10.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p15.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p15.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p20.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p20.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p30.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p30.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p40.list"
"Configurations/filelists/photons/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_FOREST_extendEC_pfSC_phiWidth0p40.list"
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
"Configurations/Performance/photons/photonRecoAna.fake.AllQCD.GED.conf"
"Configurations/Performance/photons/photonRecoAna.fake.AllQCD.SIG.GED.conf"
"Configurations/Performance/photons/photonRecoAna.fake.AllQCD.GED.conf"
"Configurations/Performance/photons/photonRecoAna.fake.AllQCD.SIG.GED.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
$outDirBase"/Performance/photons/photonRecoAna_fake_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC.root"
$outDirBase"/Performance/photons/photonRecoAna_fake_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC.root"
$outDirBase"/Performance/photons/photonRecoAna_fake_SIG_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC.root"
$outDirBase"/Performance/photons/photonRecoAna_fake_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC.root"
$outDirBase"/Performance/photons/photonRecoAna_fake_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p10.root"
$outDirBase"/Performance/photons/photonRecoAna_fake_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p10.root"
$outDirBase"/Performance/photons/photonRecoAna_fake_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p15.root"
$outDirBase"/Performance/photons/photonRecoAna_fake_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p15.root"
$outDirBase"/Performance/photons/photonRecoAna_fake_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p20.root"
$outDirBase"/Performance/photons/photonRecoAna_fake_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p20.root"
$outDirBase"/Performance/photons/photonRecoAna_fake_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p30.root"
$outDirBase"/Performance/photons/photonRecoAna_fake_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p30.root"
$outDirBase"/Performance/photons/photonRecoAna_fake_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p40.root"
$outDirBase"/Performance/photons/photonRecoAna_fake_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB_extendEC_pfSC_phiWidth0p40.root"
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

