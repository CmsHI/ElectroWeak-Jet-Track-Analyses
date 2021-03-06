#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.SysVar.sh
####################
echo "### SKIM STEP"
echo "## zJetSkim HI DATA ZEE"
####################
configFile=$config_HI_ZEE_DATA
inputSkim=$HI_ZEE_DATA
outputSkimPrefix=$skim_HI_ZEE_DATA_Prefix

outputSkim=$outputSkimDir$outputSkimPrefix".root"
outputSkimLOG=$outputSkimDirLOG$outputSkimPrefix".log"
echo "inputSkim     : "$inputSkim; echo "outputSkim    : "$outputSkim;echo "outputSkimLOG : "$outputSkimLOG
echo "./ShellScripts/myRun.sh ForestSkimmers/Zbosons/zJetSkim.exe $configFile $inputSkim $outputSkim $inputMinBiasJetSkim &> $outputSkimLOG &"
./ShellScripts/myRun.sh ForestSkimmers/Zbosons/zJetSkim.exe $configFile $inputSkim $outputSkim $inputMinBiasJetSkim &> $outputSkimLOG &

####################
DO_SYSVAR=$DO_SYSVAR_HI_ZEE_DATA
if [ $DO_SYSVAR -eq 1 ]; then 
  echo "################# SYSTEMATICS #######################"
  echo "### SKIM STEP"
  echo "## zJetSkim HI DATA ZEE"

  configSuffixList=("${configSuffixList_skim_HI_ZEE_DATA[@]}")

  cutList=("${cutList_skim_HI_ZEE_DATA[@]}")

  runZJetSkim $configFile $inputSkim $outputSkimPrefix $configSuffixList $cutList $inputMinBiasJetSkim
  echo "################# SYSTEMATICS - END #######################"
fi
####################

