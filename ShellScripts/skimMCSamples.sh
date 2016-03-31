#!/bin/bash

set -x

for PTHAT in 30 50 80 120 170
do
    INFILE=/mnt/hadoop/cms/store/user/luck/2015-PbPb-MC/Pythia8_EmEnrichedDijet${PTHAT}/0.root 
    OUTFILE=PbPb_EmEnrichedDijet${PTHAT}_eta3.phoSkim.root
    ../ForestSkimmers/photons/ggHiNtuplizerSkim.exe CutConfigurations/gamma-jet-nominal-mc.conf $INFILE /export/d00/scratch/luck/GAMMAJETFILES/mcSkim/$OUTFILE
    OUTFILE2=${OUTFILE/.phoSkim.root/.phoCorrected.root}
    ../Corrections/photonCorrections.exe CutConfigurations/gamma-jet-nominal-mc.conf /export/d00/scratch/luck/GAMMAJETFILES/mcSkim/$OUTFILE /export/d00/scratch/luck/GAMMAJETFILES/mcSkim/$OUTFILE2 
done
