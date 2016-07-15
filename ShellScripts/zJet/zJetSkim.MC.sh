#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.SysVar.sh
###################### 04.07.2016 ####################
echo "#### SKIM STEP"
echo "### zJetSkim for ALL MC"
####################
## Pythia
./ShellScripts/zJet/zJetSkim.HIMC.Zee.Pythia.sh
./ShellScripts/zJet/zJetSkim.HIMC.Zmm.Pythia.sh
./ShellScripts/zJet/zJetSkim.PPMC.Zee.Pythia.sh
./ShellScripts/zJet/zJetSkim.PPMC.Zmm.Pythia.sh

## Pyquen
./ShellScripts/zJet/zJetSkim.HIMC.Zee.Pyquen.sh
./ShellScripts/zJet/zJetSkim.HIMC.Zmm.Pyquen.sh

## Madgraph
./ShellScripts/zJet/zJetSkim.PPMC.Zee.Madgraph.sh
./ShellScripts/zJet/zJetSkim.PPMC.Zmm.Madgraph.sh

