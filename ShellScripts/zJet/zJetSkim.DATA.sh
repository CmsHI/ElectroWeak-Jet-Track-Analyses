#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
###################### 04.07.2016 ####################
echo "#### SKIM STEP"
echo "### zJetSkim for ALL DATA"
####################
./ShellScripts/zJet/zJetSkim.HI.Zee.sh
./ShellScripts/zJet/zJetSkim.HI.Zmm.sh
./ShellScripts/zJet/zJetSkim.PP.Zee.sh
./ShellScripts/zJet/zJetSkim.PP.Zmm.sh

