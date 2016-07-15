#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.SysVar.sh
###################### 04.07.2016 ####################
echo "### SKIM STEP"
echo "## zJetSkim HI DATA ZMM"
####################
configFile=$config_HI_ZMM_DATA
inputSkim=$HI_ZMM_DATA
outputSkimPrefix=$skim_HI_ZMM_DATA_Prefix

outputSkim=$outputSkimDir$outputSkimPrefix".root"
outputSkimLOG=$outputSkimDirLOG$outputSkimPrefix".log"
echo "inputSkim     : "$inputSkim; echo "outputSkim    : "$outputSkim;echo "outputSkimLOG : "$outputSkimLOG
echo "~/code/scripts/myRun.sh ForestSkimmers/Zbosons/zJetSkim.exe $configFile $inputSkim $outputSkim $inputMinBiasJetSkim &> $outputSkimLOG &"
~/code/scripts/myRun.sh ForestSkimmers/Zbosons/zJetSkim.exe $configFile $inputSkim $outputSkim $inputMinBiasJetSkim &> $outputSkimLOG &

####################
DO_SYSVAR=$DO_SYSVAR_HI_ZMM_DATA
if [ $DO_SYSVAR -eq 1 ]; then 
  echo "################# SYSTEMATICS #######################"
  echo "### SKIM STEP"
  echo "## zJetSkim HI DATA ZMM"

  configSuffixList=$configSuffixList_skim_HI_ZMM_DATA

  cutList=$cutList_skim_HI_ZMM_DATA

  runZJetSkim $configFile $inputSkim $outputSkimPrefix $configSuffixList $cutList $inputMinBiasJetSkim
  echo "################# SYSTEMATICS - END #######################"
fi
####################

