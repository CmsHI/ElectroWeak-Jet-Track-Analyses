#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.SysVar.sh
###################### 04.07.2016 ####################
echo "### SKIM STEP"
echo "## zJetSkim HI MC Pythia ZMM"
####################
configFile=$config_HI_ZMM_MC
inputSkim=$HI_ZMM_MC_Pythia
outputSkimPrefix=$skim_HI_ZMM_MC_Pythia_Prefix

outputSkim=$outputSkimDir$outputSkimPrefix".root"
outputSkimLOG=$outputSkimDirLOG$outputSkimPrefix".log"
echo "inputSkim     : "$inputSkim; echo "outputSkim    : "$outputSkim;echo "outputSkimLOG : "$outputSkimLOG
echo "~/code/scripts/myRun.sh ForestSkimmers/Zbosons/zJetSkim.exe $configFile $inputSkim $outputSkim $inputMinBiasJetSkimHydjet &> $outputSkimLOG &"
~/code/scripts/myRun.sh ForestSkimmers/Zbosons/zJetSkim.exe $configFile $inputSkim $outputSkim $inputMinBiasJetSkimHydjet &> $outputSkimLOG &

####################
DO_SYSVAR=$DO_SYSVAR_HI_ZMM_MC_Pythia
if [ $DO_SYSVAR -eq 1 ]; then 
  echo "################# SYSTEMATICS #######################"
  echo "### SKIM STEP"
  echo "## zJetSkim HI MC Pythia ZMM"

  configSuffixList=$configSuffixList_skim_HI_ZMM_MC

  cutList=$cutList_skim_HI_ZMM_MC

  runZJetSkim $configFile $inputSkim $outputSkimPrefix $configSuffixList $cutList $inputMinBiasJetSkimHydjet
  echo "################# SYSTEMATICS - END #######################"
fi
####################

