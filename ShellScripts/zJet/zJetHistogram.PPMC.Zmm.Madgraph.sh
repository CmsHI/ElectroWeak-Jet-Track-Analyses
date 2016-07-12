#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
###################### 04.07.2016 ####################
echo "### PPSTOGRAM STEP"
echo "## zJetHistogram PP MC Madgraph ZMM"
####################
configFile=$config_PP_ZMM_MC_Madgraph

outputSkimPrefix=$skim_PP_ZMM_MC_Madgraph_Prefix
outputSkim=$outputSkimDir$outputSkimPrefix".root"
inputHist=$outputSkim

outputHistPrefix=$hist_PP_ZMM_MC_Madgraph_Prefix
outputHist=$outputHistDir$outputHistPrefix".root"
outputHistLOG=$outputHistDirLOG$outputHistPrefix".log"
echo "inputHist     : "$inputHist; echo "outputHist    : "$outputHist; echo "outputLOGHist : "$outputHistLOG
echo "~/code/scripts/myRun.sh Histogramming/zBosons/zJetHistogram.exe $configFile $inputHist $outputHist &> $outputHistLOG &"
~/code/scripts/myRun.sh Histogramming/zBosons/zJetHistogram.exe $configFile $inputHist $outputHist &> $outputHistLOG &

####################
DO_SYSVAR=$DO_SYSVAR_PP_ZMM_MC_Madgraph
if [ $DO_SYSVAR -eq 1 ]; then 
  echo "################# SYSTEMATICS #######################"
  echo "### PPSTOGRAM STEP"
  echo "## zJetHistogram PP MC Madgraph ZMM"

  configSuffixList=(
#"SYS_noCorrEle"
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
#"# skim.electron.doCorrection = 0"
"# skim.jet.energyScale = 1.02"
"# skim.jet.energyScale = 0.98"
"# skim.jet.doCorrectionSmearing = 2 \n skim.jet.doCorrectionSmearingPhi = 2"
"# skim.jet.doCorrectionSmearing = 3 \n skim.jet.doCorrectionSmearingPhi = 3"
#"# skim.jet.doCorrectionSmearing = 0"
#"# skim.jet.doCorrectionSmearingPhi = 0"
#"# skim.jet.doCorrectionSmearing = 0 \n skim.jet.doCorrectionSmearingPhi = 0"
#"# skim.jet.doCorrectionL2L3 = 0"
  );

  configSuffixListSkim=(
#"SYS_noCorrEle"
"SYS_jes102"
"SYS_jes098"
"SYS_corrJetSmearPlus"
"SYS_corrJetSmearMinus"
#"SYS_noCorrJetSmear"
#"SYS_noCorrJetSmearPhi"
#"SYS_noCorrJetSmearALL"
#"SYS_noL2L3"
  );

  runZJetHistogram $configFile $outputSkimPrefix $configSuffixList $cutList $configSuffixListSkim
  echo "################# SYSTEMATICS - END #######################"
fi
####################

####################
DO_ANAVAR=0
if [ $DO_ANAVAR -eq 1 ]; then 
  echo "################# ANALYSIS VARIATIONS #######################"
  echo "### PPSTOGRAM STEP"
  echo "## zJetHistogram PP MC Madgraph ZMM"

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

