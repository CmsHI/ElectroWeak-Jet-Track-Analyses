#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
###################### 04.07.2016 ####################
echo "### HISTOGRAM STEP"
echo "## zJetHistogram HI MC Pythia ZMM"
####################
configFile=$config_HI_ZMM_MC_Pythia

outputSkimPrefix=$skim_HI_ZMM_MC_Pythia_Prefix
outputSkim=$outputSkimDir$outputSkimPrefix".root"
inputHist=$outputSkim

outputHistPrefix=$hist_HI_ZMM_MC_Pythia_Prefix
outputHist=$outputHistDir$outputHistPrefix".root"
outputHistLOG=$outputHistDirLOG$outputHistPrefix".log"
echo "inputHist     : "$inputHist; echo "outputHist    : "$outputHist; echo "outputLOGHist : "$outputHistLOG
echo "~/code/scripts/myRun.sh Histogramming/zBosons/zJetHistogram.exe $configFile $inputHist $outputHist &> $outputHistLOG &"
~/code/scripts/myRun.sh Histogramming/zBosons/zJetHistogram.exe $configFile $inputHist $outputHist &> $outputHistLOG &

####################
DO_SYSVAR=$DO_SYSVAR_HI_ZMM_MC_Pythia
if [ $DO_SYSVAR -eq 1 ]; then 
  echo "################# SYSTEMATICS #######################"
  echo "### HISTOGRAM STEP"
  echo "## zJetHistogram HI MC Pythia ZMM"

  configSuffixList=(
"SYS_jes106"
"SYS_jes096"
#"SYS_noCorrJetRes"
"SYS_smearResJetPlus"
"SYS_smearResJetMinus"
#"SYS_noJetID"
#"SYS_noJetIDCorr"
  );

  cutList=(
"# skim.jet.energyScale = 1.06"
"# skim.jet.energyScale = 0.96"
#"# skim.jet.doCorrectionResidual = 0"
"# skim.jet.smearingRes = 1.15"
"# skim.jet.smearingRes = 0.85"
#"histogram.jet.jetID = 0 \n histogram.jet.doCorrectionJetID = 0"
#"histogram.jet.jetID = 1 \n histogram.jet.doCorrectionJetID = 0"
  );

  configSuffixListSkim=(
"SYS_jes106"
"SYS_jes096"
#"SYS_noCorrJetRes"
"SYS_smearResJetPlus"
"SYS_smearResJetMinus"
#""
#""
  );

  runZJetHistogram $configFile $outputSkimPrefix $configSuffixList $cutList $configSuffixListSkim
  echo "################# SYSTEMATICS - END #######################"
fi
####################

####################
DO_ANAVAR=0
if [ $DO_ANAVAR -eq 1 ]; then 
  echo "################# ANALYSIS VARIATIONS #######################"
  echo "### HISTOGRAM STEP"
  echo "## zJetHistogram HI MC Pythia ZMM"

  configSuffixList=(
"jteta16"
"jteta2"
"jtpt20"
"jtpt25"
"jetID"
  );

  cutList=(
"histogram.jet.eta = 1.6"
"histogram.jet.eta = 2"
"histogram.jet.pt = 20"
"histogram.jet.pt = 25"
"histogram.jet.jetID = 1 \n histogram.jet.doCorrectionJetID = 1"
  );

  configSuffixListSkim=(
""
""
""
""
""
  );

  runZJetHistogram $configFile $outputSkimPrefix $configSuffixList $cutList $configSuffixListSkim
  echo "################# ANALYSIS VARIATIONS - END #######################"
fi
####################

