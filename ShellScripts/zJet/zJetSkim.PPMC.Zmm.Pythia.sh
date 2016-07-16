#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.SysVar.sh
####################
echo "### SKIM STEP"
echo "## zJetSkim PP MC Pythia ZMM"
####################
configFile=$config_PP_ZMM_MC
inputSkim=$PP_ZMM_MC_Pythia
outputSkimPrefix=$skim_PP_ZMM_MC_Pythia_Prefix

outputSkim=$outputSkimDir$outputSkimPrefix".root"
outputSkimLOG=$outputSkimDirLOG$outputSkimPrefix".log"
echo "inputSkim     : "$inputSkim; echo "outputSkim    : "$outputSkim;echo "outputSkimLOG : "$outputSkimLOG
echo "~/code/scripts/myRun.sh ForestSkimmers/Zbosons/zJetSkim.exe $configFile $inputSkim $outputSkim &> $outputSkimLOG &"
~/code/scripts/myRun.sh ForestSkimmers/Zbosons/zJetSkim.exe $configFile $inputSkim $outputSkim &> $outputSkimLOG &

####################
DO_SYSVAR=$DO_SYSVAR_PP_ZMM_MC_Pythia
if [ $DO_SYSVAR -eq 1 ]; then 
  echo "################# SYSTEMATICS #######################"
  echo "### SKIM STEP"
  echo "## zJetSkim PP MC Pythia ZMM"

  configSuffixList=("${configSuffixList_skim_PP_ZMM_MC[@]}")

  cutList=("${cutList_skim_PP_ZMM_MC[@]}")

  runZJetSkim $configFile $inputSkim $outputSkimPrefix $configSuffixList $cutList
  echo "################# SYSTEMATICS - END #######################"
fi
####################

