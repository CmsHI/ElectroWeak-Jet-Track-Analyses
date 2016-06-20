#!/bin/bash

source ./ShellScripts/bashUtils.sh

# example usage of runGeneratedConf() on plotting dphi correlation in gammaJet 
progPath="./Plotting/plotHistogram.exe"
importedConfig="./CutConfigurations/gammaJetPlotConfigs/dphi_ptBin0_hiBin0.conf"
configsCut="###"     # do not set to empty string
configsInput="###"     # do not set to empty string
inputFile=$importedConfig
outputFile=""
logFile=""

ptBinsListTEXT=(
"p_{T}^{\gamma} > 40 GeV/c"
"p_{T}^{\gamma} > 60 GeV/c"
"40 GeV/c < p_{T}^{\gamma} < 50 GeV/c"
"50 GeV/c < p_{T}^{\gamma} < 60 GeV/c"
"60 GeV/c < p_{T}^{\gamma} < 80 GeV/c"
"p_{T}^{\gamma} > 80 GeV/c"
"80 GeV/c < p_{T}^{\gamma} < 100 GeV/c"
"p_{T}^{\gamma} > 100 GeV/c"
);

hiBinsListTEXT=(
"0-100%"
"0-30%"
"30-100%"
"0-10%"
"10-30%"
"30-50%"
"50-100%"
);


for iPt in {0..7}
do
  for iHiBin in {0..6}
  do
    # prepared modification to the imported configuration
    line1="input.plotting.TH1_path = {HI/h1D_dphi_ptBin"${iPt}"_hiBin"${iHiBin}"_phoSIG_jetSIG_final_norm, HIMC/h1D_dphi_ptBin"${iPt}"_hiBin"${iHiBin}"_phoSIG_jetSIG_final_norm, PP/h1D_dphi_ptBin"${iPt}"_hiBin"${iHiBin}"_phoSIG_jetSIG_final_norm}"
    line2="input.plotting.text = {"${hiBinsListTEXT[$iHiBin]}";; "${ptBinsListTEXT[$iPt]}";; p_{T}^{Jet} > 30 GeV/c,  |\\\eta^{Jet}| < 1.6;;|\\\eta^{\gamma}| < 1.44;; akPu3PFJets}"
    configsCut="###"     # cut configuration is not modified, nevertheless do not set it to empty string
    configsInput=$line1" \n "$line2     # the whole configuration is contained in a single string where the lines are separated by "\n"

    outputPrefix="CutConfigurations/gammaJetPlotConfigs/dphi_ptBin"${iPt}"_hiBin${iHiBin}"
    outputFile=$outputPrefix".png"
    logFile=$outputPrefix".log"

    # plot dphi histograms
    runGeneratedConf $progPath $importedConfig $configsCut $configsInput $inputFile $outputFile $logFile
  done
done
