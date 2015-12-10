#!/bin/bash

#make clean
set -x

make
#./ForestSkimmers/photons/ggHiNtuplizerSkim.exe CutConfigurations/gamma-jet-nominal.conf  root://eoscms//eos/cms/store/group/phys_heavyions/velicanu/forest/HIRun2015/HIHardProbes/Merged/HiForestPromptRecoHIHardProbes_262726.root ~/workspace/PhoSkim_PromptRecoHIHardProbes_262726.root || exit

#./Corrections/photonCorrections.exe CutConfigurations/gamma-jet-nominal.conf ~/workspace/PhoSkim_PromptRecoHIHardProbes_262726.root ~/workspace/PhoSkimCorrected_PromptRecoHIHardProbes_262726.root || exit

./Histogramming/quickPhotonPurity.exe CutConfigurations/gamma-jet-nominal.conf ~/workspace/PhoSkimCorrected_PromptRecoHIHardProbes_262726.root ~/workspace/Pyquen_AllQCDPhoton30_phoSkimCorrected.root ./purityHists.root || exit
