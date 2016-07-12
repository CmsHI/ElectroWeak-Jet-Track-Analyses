#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
###################### 04.07.2016 ####################
echo "### SKIM STEP"
echo "## zJetSkim HI MC Pyquen ZEE"
####################
configFile=$config_HI_ZEE_MC
inputSkim=$HI_ZEE_MC_Pyquen
outputSkimPrefix=$skim_HI_ZEE_MC_Pyquen_Prefix

outputSkim=$outputSkimDir$outputSkimPrefix".root"
outputSkimLOG=$outputSkimDirLOG$outputSkimPrefix".log"
echo "inputSkim     : "$inputSkim; echo "outputSkim    : "$outputSkim;echo "outputSkimLOG : "$outputSkimLOG
echo "~/code/scripts/myRun.sh ForestSkimmers/Zbosons/zJetSkim.exe $configFile $inputSkim $outputSkim $inputMinBiasJetSkimHydjet &> $outputSkimLOG &"
~/code/scripts/myRun.sh ForestSkimmers/Zbosons/zJetSkim.exe $configFile $inputSkim $outputSkim $inputMinBiasJetSkimHydjet &> $outputSkimLOG &

####################
DO_SYSVAR=$DO_SYSVAR_HI_ZEE_MC_Pyquen
if [ $DO_SYSVAR -eq 1 ]; then 
  echo "################# SYSTEMATICS #######################"
  echo "### SKIM STEP"
  echo "## zJetSkim HI MC Pyquen ZEE"

  configSuffixList=(
#"SYS_noCorrEle"
"SYS_jes106"
"SYS_jes096"
"SYS_eesPlus"
"SYS_eesMinus"
#"SYS_noCorrJetRes"
#"SYS_noL2L3"
"SYS_smearZ"
"SYS_smearResJetPlus"
"SYS_smearResJetMinus"
  );

  cutList=(
#"skim.electron.doCorrection = 0"
"skim.jet.energyScale = 1.06"
"skim.jet.energyScale = 0.96"
"skim.electron.energyScale = 1.005"
"skim.electron.energyScale = 0.995"
#"skim.jet.doCorrectionResidual = 0"
#"skim.jet.doCorrectionL2L3 = 0"
"skim.zboson.smearZ = 0.05"
"skim.jet.smearingRes = 0.15"
"skim.jet.smearingRes = 0.85"
  );

  runZJetSkim $configFile $inputSkim $outputSkimPrefix $configSuffixList $cutList $inputMinBiasJetSkimHydjet
  echo "################# SYSTEMATICS - END #######################"
fi
####################

