#!/bin/bash

set -x

./ForestSkimmers/photons/gammaJetSkim.exe ./CutConfigurations/gammaJet.conf ./CutConfigurations/PbPb_MC.list /export/d00/scratch/luck/GAMMAJETFILES/PbPb_MC_gammaJetSkim.root /export/d00/scratch/luck/GAMMAJETFILES/PbPb_MC_minbiasJetSkim.root
