#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.SysVar.sh
###################### 04.07.2016 ####################
echo "#### HISTOGRAM STEP"
echo "### zJetHistogram for ALL DATA"
####################
./ShellScripts/zJet/zJetHistogram.HI.Zee.sh
./ShellScripts/zJet/zJetHistogram.HI.Zmm.sh
./ShellScripts/zJet/zJetHistogram.PP.Zee.sh
./ShellScripts/zJet/zJetHistogram.PP.Zmm.sh

