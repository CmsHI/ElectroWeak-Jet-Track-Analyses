#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.SysVar.sh
####################
echo "### SKIM STEP"
echo "## zJetSkim PP MC Pythia ZEE"
####################
configFile=$config_PP_ZEE_MC
inputSkim=$PP_ZEE_MC_Pythia
outputSkimPrefix=$skim_PP_ZEE_MC_Pythia_Prefix

outputSkim=$outputSkimDir$outputSkimPrefix".root"
outputSkimLOG=$outputSkimDirLOG$outputSkimPrefix".log"
echo "inputSkim     : "$inputSkim; echo "outputSkim    : "$outputSkim;echo "outputSkimLOG : "$outputSkimLOG
### For now, PP ZEE zJetSkim cannot be run using compiled because of some missing libraries used in electron energy correction. Run PP ZEE zJetSkim in a ROOT session.
#echo "./ShellScripts/myRun.sh ForestSkimmers/Zbosons/zJetSkim.exe $configFile $inputSkim $outputSkim &> $outputSkimLOG &"
#./ShellScripts/myRun.sh ForestSkimmers/Zbosons/zJetSkim.exe $configFile $inputSkim $outputSkim &> $outputSkimLOG &
echo "root -l -b 'ForestSkimmers/Zbosons/zJetSkim.C("'$configFile'", "'$inputSkim'", "'$outputSkim'")' &> $outputSkimLOG &"
root -l -b 'ForestSkimmers/Zbosons/zJetSkim.C("'$configFile'", "'$inputSkim'", "'$outputSkim'")' &> $outputSkimLOG &

####################
DO_SYSVAR=$DO_SYSVAR_PP_ZEE_MC_Pythia
if [ $DO_SYSVAR -eq 1 ]; then 
  echo "################# SYSTEMATICS #######################"
  echo "### SKIM STEP"
  echo "## zJetSkim PP MC Pythia ZEE"

  configSuffixList=("${configSuffixList_skim_PP_ZEE_MC[@]}")

  cutList=("${cutList_skim_PP_ZEE_MC[@]}")

  runZJetSkim $configFile $inputSkim $outputSkimPrefix $configSuffixList $cutList
  echo "################# SYSTEMATICS - END #######################"
fi
####################

