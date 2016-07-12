#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
###################### 04.07.2016 ####################
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

  configSuffixList=(
"SYS_jes102"
"SYS_jes098"
"SYS_corrJetSmearPlus"
"SYS_corrJetSmearMinus"
#"SYS_noCorrJetSmear"
#"SYS_noCorrJetSmearPhi"
"SYS_noCorrJetSmearALL"
#"SYS_noL2L3"
  );

  cutList=(
"skim.jet.energyScale = 1.02"
"skim.jet.energyScale = 0.98"
"skim.jet.doCorrectionSmearing = 2 \n skim.jet.doCorrectionSmearingPhi = 2"
"skim.jet.doCorrectionSmearing = 3 \n skim.jet.doCorrectionSmearingPhi = 3"
#"skim.jet.doCorrectionSmearing = 0"
#"skim.jet.doCorrectionSmearingPhi = 0"
"skim.jet.doCorrectionSmearing = 0 \n skim.jet.doCorrectionSmearingPhi = 0"
#"skim.jet.doCorrectionL2L3 = 0"
  );

  runZJetSkim $configFile $inputSkim $outputSkimPrefix $configSuffixList $cutList
  echo "################# SYSTEMATICS - END #######################"
fi
####################

