#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.SysVar.sh
###################### 04.07.2016 ####################
echo "### HISTOGRAM STEP"
echo "## zJetHistogram HI MC Pyquen ZEE"
####################
configFile=$config_HI_ZEE_MC_Pyquen

outputSkimPrefix=$skim_HI_ZEE_MC_Pyquen_Prefix
outputSkim=$outputSkimDir$outputSkimPrefix".root"
inputHist=$outputSkim

outputHistPrefix=$hist_HI_ZEE_MC_Pyquen_Prefix
outputHist=$outputHistDir$outputHistPrefix".root"
outputHistLOG=$outputHistDirLOG$outputHistPrefix".log"
echo "inputHist     : "$inputHist; echo "outputHist    : "$outputHist; echo "outputLOGHist : "$outputHistLOG
echo "~/code/scripts/myRun.sh Histogramming/zBosons/zJetHistogram.exe $configFile $inputHist $outputHist &> $outputHistLOG &"
~/code/scripts/myRun.sh Histogramming/zBosons/zJetHistogram.exe $configFile $inputHist $outputHist &> $outputHistLOG &

####################
DO_SYSVAR=$DO_SYSVAR_HI_ZEE_MC_Pyquen
if [ $DO_SYSVAR -eq 1 ]; then 
  echo "################# SYSTEMATICS #######################"
  echo "### HISTOGRAM STEP"
  echo "## zJetHistogram HI MC Pyquen ZEE"

  configSuffixList=$configSuffixList_hist_HI_ZEE_MC

  cutList=$cutList_hist_HI_ZEE_MC

  configSuffixListSkim=$skimSuffixList_hist_HI_ZEE_MC

  runZJetHistogram $configFile $outputSkimPrefix $configSuffixList $cutList $configSuffixListSkim
  echo "################# SYSTEMATICS - END #######################"
fi
####################

####################
DO_ANAVAR=0
if [ $DO_ANAVAR -eq 1 ]; then 
  echo "################# ANALYSIS VARIATIONS #######################"
  echo "### HISTOGRAM STEP"
  echo "## zJetHistogram HI MC Pyquen ZEE"

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

