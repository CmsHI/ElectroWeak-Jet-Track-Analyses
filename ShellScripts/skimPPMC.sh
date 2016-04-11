#!/bin/bash

set -x

for PTHAT in 15 30 50 80 120
do
    INFILE=/mnt/hadoop/cms/store/user/rbi/merged/Pythia8_Photon${PTHAT}_pp502_TuneCUETP8M1-HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1_forest_v1/0.root 
    ../ForestSkimmers/photons/gammaJetSkim.exe ../CutConfigurations/gammaJet_pp.conf $INFILE /export/d00/scratch/luck/GAMMAJETFILES/pp_MC_pthat${PTHAT}_gammaJetSkim.root
done

