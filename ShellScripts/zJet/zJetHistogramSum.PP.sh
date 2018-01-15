#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.SysVar.sh
####################
echo "### HISTOGRAMSUM STEP"
echo "## zJetHistogramSum PP DATA"
####################
configFile=$config_histSum_PP_DATA

inputHistDir=$EWJTAOUT"/zJet20160715/"
inputHistPrefixZEE=$inputHistDir$hist_PP_ZEE_DATA_Prefix
inputHistPrefixZMM=$inputHistDir$hist_PP_ZMM_DATA_Prefix
inputZEE=$inputHistPrefixZEE".root"
inputZMM=$inputHistPrefixZMM".root"

outputPrefix=$outputHistSumDir$histSum_PP_DATA_Prefix
output=$outputPrefix".root"
outputLOG=$outputPrefix".log"

echo "./Histogramming/zBosons/zJetHistogramSum.exe $configFile $inputZEE $inputZMM $output &> $outputLOG &"
./Histogramming/zBosons/zJetHistogramSum.exe $configFile $inputZEE $inputZMM $output &> $outputLOG &

####################
configSuffixListZEE=("${configSuffixList_histSum_PP_ZEE_DATA[@]}")

configSuffixListZMM=("${configSuffixList_histSum_PP_ZMM_DATA[@]}")

DO_SYS_ZEE=$DO_SYSVAR_PP_ZEE_DATA
DO_SYS_ZMM=$DO_SYSVAR_PP_ZMM_DATA
if [[ $DO_SYS_ZEE -eq 1 ]] && [[ $DO_SYS_ZMM -eq 1 ]]; then 
  echo "################# SYSTEMATICS #######################"
  echo "### HISTOGRAM STEP"
  echo "## zJetHistogramSum PP DATA"

  runZJetHistogramSum $configFile $outputPrefix $inputHistPrefixZEE $inputHistPrefixZMM $configSuffixListZEE $configSuffixListZMM
  echo "################# SYSTEMATICS - END #######################"
fi

