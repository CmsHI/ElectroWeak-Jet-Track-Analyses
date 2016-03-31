#!/bin/bash

set -x

#INFILE=/mnt/hadoop/cms/store/user/richard/2015-Data-promptRECO-photonSkims/HIPhoton40AndZ/PbPb-photonHLTFilter-v3/160202_145715/000${NUM}/*.root
INFILE=bigSkim.txt
OUTFILE=PbPb_Data_gammaJetSkim.root
../ForestSkimmers/photons/gammaJetSkim.exe ../CutConfigurations/gammaJet.conf $INFILE /export/d00/scratch/luck/GAMMAJETFILES/$OUTFILE /export/d00/scratch/luck/GAMMAJETFILES/minBiasJetSkim/minbiasJetSkim.root
