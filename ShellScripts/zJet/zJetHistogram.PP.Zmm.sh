#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.SysVar.sh
####################
echo "### HISTOGRAM STEP"
echo "## zJetHistogram PP DATA ZMM"
####################
configFile=$config_PP_ZMM_DATA

outputSkimPrefix=$skim_PP_ZMM_DATA_Prefix
outputSkim=$outputSkimDir$outputSkimPrefix".root"
inputHist=$outputSkim

outputHistPrefix=$hist_PP_ZMM_DATA_Prefix
outputHist=$outputHistDir$outputHistPrefix".root"
outputHistLOG=$outputHistDirLOG$outputHistPrefix".log"
echo "inputHist     : "$inputHist; echo "outputHist    : "$outputHist; echo "outputLOGHist : "$outputHistLOG
echo "./ShellScripts/myRun.sh Histogramming/zBosons/zJetHistogram.exe $configFile $inputHist $outputHist &> $outputHistLOG &"
./ShellScripts/myRun.sh Histogramming/zBosons/zJetHistogram.exe $configFile $inputHist $outputHist &> $outputHistLOG &

####################
DO_SYSVAR=$DO_SYSVAR_PP_ZMM_DATA
if [ $DO_SYSVAR -eq 1 ]; then 
  echo "################# SYSTEMATICS #######################"
  echo "### HISTOGRAM STEP"
  echo "## zJetHistogram PP DATA ZMM"

  configSuffixList=("${configSuffixList_hist_PP_ZMM_DATA[@]}")

  cutList=("${cutList_hist_PP_ZMM_DATA[@]}")

  configSuffixListSkim=("${skimSuffixList_hist_PP_ZMM_DATA[@]}")

  runZJetHistogram $configFile $outputSkimPrefix $configSuffixList $cutList $configSuffixListSkim
  echo "################# SYSTEMATICS - END #######################"
fi
####################

####################
DO_ANAVAR=0
if [ $DO_ANAVAR -eq 1 ]; then 
  echo "################# ANALYSIS VARIATIONS #######################"
  echo "### HISTOGRAM STEP"
  echo "## zJetHistogram PP DATA ZMM"

  configSuffixList=(
"jteta16"
"jteta2"
"jtpt20"
"jtpt25"
"smearHiBin2"  # 30-100% smearing
  );

  cutList=(
"histogram.jet.eta = 1.6"
"histogram.jet.eta = 2"
"histogram.jet.pt = 20"
"histogram.jet.pt = 25"
"# skim.jet.smearingHiBin = 2"
  );

  configSuffixListSkim=(
""
""
""
""
"smearHiBin2"
  );

  runZJetHistogram $configFile $outputSkimPrefix $configSuffixList $cutList $configSuffixListSkim
  echo "################# ANALYSIS VARIATIONS - END #######################"
fi
####################

