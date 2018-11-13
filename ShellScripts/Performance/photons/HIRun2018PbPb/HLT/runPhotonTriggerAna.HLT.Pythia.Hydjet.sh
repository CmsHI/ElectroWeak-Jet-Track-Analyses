#!/bin/bash

forestAllQCDPhoton15="root://xrootd.cmsaf.mit.edu//store/user/katatar/HIRun2018PbPb/Pythia8_AllQCDPhoton15_bias_Hydjet_Drum5Ev8_5020GeV/CMSSW_10_3_0-103X_upgrade2018_realistic_HI_v7-FOREST/merged.root"

openHLT_hltTestEgammaV70_L1_defaultSK="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton15_bias_Hydjet_Drum5Ev8_5020GeV/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_L1_defaultSK/openHLT_merged.root"
openHLT_hltTestEgammaV70_L1_SK1212="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton15_bias_Hydjet_Drum5Ev8_5020GeV/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_L1_SK1212/openHLT_merged.root"
openHLT_hltTestEgammaV70_L1_SK1207="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton15_bias_Hydjet_Drum5Ev8_5020GeV/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V70_L1_SK1207/openHLT_merged.root"
openHLT_hltTestEgammaV73_L1_SK1207="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton15_bias_Hydjet_Drum5Ev8_5020GeV/103X_upgrade2018_realistic_HI_v7_hltTestEgamma_V73_L1_SK1207/openHLT_merged.root"

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonTriggerAna.exe"
triggerFiles=(
$openHLT_hltTestEgammaV73_L1_SK1207
$openHLT_hltTestEgammaV73_L1_SK1207
$openHLT_hltTestEgammaV73_L1_SK1207
$openHLT_hltTestEgammaV73_L1_SK1207
$openHLT_hltTestEgammaV73_L1_SK1207
## L1Seeded
$openHLT_hltTestEgammaV73_L1_SK1207
$openHLT_hltTestEgammaV73_L1_SK1207
## PhotonXX
$openHLT_hltTestEgammaV73_L1_SK1207
$openHLT_hltTestEgammaV73_L1_SK1207
$openHLT_hltTestEgammaV73_L1_SK1207
$openHLT_hltTestEgammaV73_L1_SK1207
$openHLT_hltTestEgammaV73_L1_SK1207
$openHLT_hltTestEgammaV73_L1_SK1207
);

inputList=(
$forestAllQCDPhoton15
$forestAllQCDPhoton15
$forestAllQCDPhoton15
$forestAllQCDPhoton15
$forestAllQCDPhoton15
## L1Seeded
$forestAllQCDPhoton15
$forestAllQCDPhoton15
## PhotonXX
$forestAllQCDPhoton15
$forestAllQCDPhoton15
$forestAllQCDPhoton15
$forestAllQCDPhoton15
$forestAllQCDPhoton15
$forestAllQCDPhoton15
);

configList=(
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.HLT.Pythia.Hydjet.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.HLT.Pythia.Hydjet.EB.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.HLT.Pythia.Hydjet.HIIslandPhoton.Eta3p1.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.HLT.Pythia.Hydjet.HIIslandPhoton.Eta2p4.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.HLT.Pythia.Hydjet.HIIslandPhoton.Eta1p5.conf"
## L1Seeded
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.HLT.Pythia.Hydjet.L1Seeded.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.HLT.Pythia.Hydjet.L1Seeded.EB.conf"
## PhotonXX
"Configurations/Performance/photons/HIRun2018PbPb/HLT/PhotonXX/photonTriggerAna.HLT.Pythia.Hydjet.10.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/PhotonXX/photonTriggerAna.HLT.Pythia.Hydjet.10.EB.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/PhotonXX/photonTriggerAna.HLT.Pythia.Hydjet.20.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/PhotonXX/photonTriggerAna.HLT.Pythia.Hydjet.20.EB.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/PhotonXX/photonTriggerAna.HLT.Pythia.Hydjet.30.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/PhotonXX/photonTriggerAna.HLT.Pythia.Hydjet.30.EB.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffixAllQCDPhoton15="Pythia8_AllQCDPhoton15_bias_Hydjet_Drum5Ev8_5020GeV"
outputSuffix="Hydjet_Quenched_Cymbal5Ev8"
#outputSuffix="Hydjet_Quenched_Cymbal5Ev8_matchHLTobj"
outList=(
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_HIGEDPhoton_"$outputSuffixAllQCDPhoton15"_hltTestEgamma_V73_L1_SK1207.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_HIGEDPhoton_EB_"$outputSuffixAllQCDPhoton15"_hltTestEgamma_V73_L1_SK1207.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_HIIslandPhoton_Eta3p1_"$outputSuffixAllQCDPhoton15"_hltTestEgamma_V73_L1_SK1207.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_HIIslandPhoton_Eta2p4_"$outputSuffixAllQCDPhoton15"_hltTestEgamma_V73_L1_SK1207.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_HIIslandPhoton_Eta1p5_"$outputSuffixAllQCDPhoton15"_hltTestEgamma_V73_L1_SK1207.root"
## L1Seeded
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_HIGEDPhoton_L1Seeded_"$outputSuffixAllQCDPhoton15"_hltTestEgamma_V73_L1_SK1207.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_HIGEDPhoton_EB_L1Seeded_"$outputSuffixAllQCDPhoton15"_hltTestEgamma_V73_L1_SK1207.root"
## PhotonXX
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_Photon10_"$outputSuffixAllQCDPhoton15"_hltTestEgamma_V73_L1_SK1207.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_Photon10_EB_"$outputSuffixAllQCDPhoton15"_hltTestEgamma_V73_L1_SK1207.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_Photon20_"$outputSuffixAllQCDPhoton15"_hltTestEgamma_V73_L1_SK1207.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_Photon20_EB_"$outputSuffixAllQCDPhoton15"_hltTestEgamma_V73_L1_SK1207.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_Photon30_"$outputSuffixAllQCDPhoton15"_hltTestEgamma_V73_L1_SK1207.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_Photon30_EB_"$outputSuffixAllQCDPhoton15"_hltTestEgamma_V73_L1_SK1207.root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    triggerFile=${triggerFiles[i1]}
    inputFile=${inputList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $triggerFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $triggerFile $inputFile $outputFile &> $outputFileLOG &"
done

