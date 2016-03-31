#!/bin/bash

#set -x

for INFILE in /mnt/hadoop/cms/store/user/richard/2015-Data-promptRECO-photonSkims/HIPhoton40AndZ/PbPb-photonHLTFilter-v3/160202_145715/*/*.root
do
     OUTFILE=$(basename $INFILE)_eta3.phoSkim.root
     ./ForestSkimmers/photons/ggHiNtuplizerSkim.exe CutConfigurations/gamma-jet-nominal.conf $INFILE /export/d00/scratch/luck/GAMMAJETFILES/bigSkim/$OUTFILE
done

hadd /export/d00/scratch/luck/GAMMAJETFILES/PbPb_promptReco_Data_eta3_phoSkim.root /export/d00/scratch/luck/GAMMAJETFILES/bigSkim/*_eta3.phoSkim.root

./Corrections/photonCorrections.exe CutConfigurations/gamma-jet-nominal.conf /export/d00/scratch/luck/GAMMAJETFILES/PbPb_promptReco_Data_eta3_phoSkim.root /export/d00/scratch/luck/GAMMAJETFILES/PbPb_promptReco_Data_eta3_phoCorrected.root
