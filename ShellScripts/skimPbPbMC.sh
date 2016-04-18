#!/bin/bash

set -x

count=0
for INFILE in /mnt/hadoop/cms/store/user/rbi/merged/Pythia8_Photon15_Hydjet_MB-HINPbPbWinter16DR-75X_mcRun2_HeavyIon_forest_v1/0.root  /mnt/hadoop/cms/store/user/rbi/merged/Pythia8_Photon30_Hydjet_MB-HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1_forest_v1/0.root  /mnt/hadoop/cms/store/user/rbi/merged/Pythia8_Photon50_Hydjet_MB-HINPbPbWinter16DR-75X_mcRun2_HeavyIon_forest_v1/0.root  /mnt/hadoop/cms/store/user/rbi/merged/Pythia8_Photon80_Hydjet_MB-HINPbPbWinter16DR-75X_mcRun2_HeavyIon_forest_v1/0.root  /mnt/hadoop/cms/store/user/rbi/merged/Pythia8_Photon120_Hydjet_MB-HINPbPbWinter16DR-75X_mcRun2_HeavyIon_forest_v1/0.root 
do
    ../ForestSkimmers/photons/gammaJetSkim.exe ../CutConfigurations/gammaJet.conf $INFILE /export/d00/scratch/luck/GAMMAJETFILES/PbPb_MC_pthat${count}_gammaJetSkim.root
    count=$((count+1))
done

