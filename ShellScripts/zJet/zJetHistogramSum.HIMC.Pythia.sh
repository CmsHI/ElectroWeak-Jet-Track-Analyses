#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.SysVar.sh
####################
echo "### HISTOGRAMSUM STEP"
echo "## zJetHistogramSum HI MC"
####################
configFile=$config_histSum_HI_MC_Pythia

inputHistDir=$EWJTAOUT"/zJet20160715/"
inputHistPrefixZEE=$inputHistDir$hist_HI_ZEE_MC_Pythia_Prefix
inputHistPrefixZMM=$inputHistDir$hist_HI_ZMM_MC_Pythia_Prefix
inputZEE=$inputHistPrefixZEE".root"
inputZMM=$inputHistPrefixZMM".root"

outputPrefix=$outputHistSumDir$histSum_HI_MC_Pythia_Prefix
output=$outputPrefix".root"
outputLOG=$outputPrefix".log"

echo "./Histogramming/zBosons/zJetHistogramSum.exe $configFile $inputZEE $inputZMM $output &> $outputLOG &"
./Histogramming/zBosons/zJetHistogramSum.exe $configFile $inputZEE $inputZMM $output &> $outputLOG &

####################
configSuffixListZEE=("${configSuffixList_histSum_HI_ZEE_MC[@]}")

configSuffixListZMM=("${configSuffixList_histSum_HI_ZMM_MC[@]}")

DO_SYS_ZEE=$DO_SYSVAR_HI_ZEE_MC_Pythia
DO_SYS_ZMM=$DO_SYSVAR_HI_ZMM_MC_Pythia
if [[ $DO_SYS_ZEE -eq 1 ]] && [[ $DO_SYS_ZMM -eq 1 ]]; then 
  echo "################# SYSTEMATICS #######################"
  echo "### HISTOGRAM STEP"
  echo "## zJetHistogramSum HI MC"

  runZJetHistogramSum $configFile $outputPrefix $inputHistPrefixZEE $inputHistPrefixZMM $configSuffixListZEE $configSuffixListZMM
  echo "################# SYSTEMATICS - END #######################"
fi

