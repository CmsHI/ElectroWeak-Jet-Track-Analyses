#!/bin/bash

set -x
set -f
INFILE=/mnt/hadoop/cms/store/user/rbi/merged/HIMinimumBias2-HIRun2015-PromptReco-v1_forest_csjet_v1/*.root
OUTFILE=minbiasJetSkim.root
mkdir -p /export/d00/scratch/luck/GAMMAJETFILES/minBiasJetSkim/
../ForestSkimmers/jets/minBiasJetSkim.exe ../CutConfigurations/gammaJet.conf $INFILE /export/d00/scratch/luck/GAMMAJETFILES/minBiasJetSkim/$OUTFILE
set +f
