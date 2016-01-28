#!/bin/bash

#make clean
set -x

#make
./ForestSkimmers/photons/ggHiNtuplizerSkim.exe CutConfigurations/gamma-jet-nominal.conf /mnt/hadoop/cms/store/user/luck/2015-Data-promptRECO-photonSkims/PbPb-photonHLTFilter-v0-HiForest/0.root  ~/scratch/GAMMAJETFILES/PbPb_Data_phoSkim.root || exit

./ForestSkimmers/photons/ggHiNtuplizerSkim.exe CutConfigurations/gamma-jet-nominal-mc.conf /mnt/hadoop/cms/store/user/luck/2015-PbPb-MC/AllQCDPhoton30-v0-HiForest/0.root  ~/scratch/GAMMAJETFILES/PbPb_MC_AllQCDPhoton30_phoSkim.root || exit

#./Corrections/photonCorrections.exe CutConfigurations/gamma-jet-nominal.conf ~/workspace/PhoSkim_PromptRecoHIHardProbes_262726.root ~/workspace/PhoSkimCorrected_PromptRecoHIHardProbes_262726.root || exit

./Histogramming/quickPhotonPurity.exe CutConfigurations/gamma-jet-nominal.conf ~/scratch/GAMMAJETFILES/PbPb_Data_phoSkim.root ~/scratch/GAMMAJETFILES/PbPb_MC_AllQCDPhoton30_phoSkim.root ./purityHists.root || exit
