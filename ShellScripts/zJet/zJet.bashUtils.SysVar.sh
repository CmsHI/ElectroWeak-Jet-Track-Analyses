#!/bin/bash

source ./ShellScripts/bashUtils.sh
##### utilities for systematic variations
#### global variables
IFS=$'\n' # very important. force to use only '\n' to split the elements of the array

# flags for running systematic variation
DO_SYSVAR_HI_ZEE_DATA=1
DO_SYSVAR_HI_ZMM_DATA=1
DO_SYSVAR_PP_ZEE_DATA=1
DO_SYSVAR_PP_ZMM_DATA=1
DO_SYSVAR_HI_ZEE_MC_Pythia=0
DO_SYSVAR_HI_ZMM_MC_Pythia=0
DO_SYSVAR_PP_ZEE_MC_Pythia=0
DO_SYSVAR_PP_ZMM_MC_Pythia=0
DO_SYSVAR_HI_ZEE_MC_Pyquen=1
DO_SYSVAR_HI_ZMM_MC_Pyquen=1
DO_SYSVAR_PP_ZEE_MC_Madgraph=0
DO_SYSVAR_PP_ZMM_MC_Madgraph=0

### arrays for analysis variations
## HI ZEE - skim
configSuffixList_skim_HI_ZEE=(
"SYS_jesPlus"
"SYS_jesMinus"
"SYS_eesPlus"
"SYS_eesMinus"
"SYS_smearResJet"
"SYS_smearResJetPhi"
"SYS_smearZ"
#"SYS_noCorrEle"
#"SYS_noCorrJetRes"
#"SYS_noL2L3"
);

cutList_skim_HI_ZEE=(
"skim.jet.energyScale = 1.06"
"skim.jet.energyScale = 0.96"
"skim.electron.energyScale = 1.005"
"skim.electron.energyScale = 0.995"
"skim.jet.smearingRes = 0.15"
"skim.jet.smearingResPhi = 0.15"
"skim.zboson.smearZ = 0.05"
#"skim.electron.doCorrection = 0"
#"skim.jet.doCorrectionResidual = 0"
#"skim.jet.doCorrectionL2L3 = 0"
);

## HI ZEE - histogram
configSuffixList_hist_HI_ZEE=(
"SYS_jesPlus"
"SYS_jesMinus"
"SYS_eesPlus"
"SYS_eesMinus"
"SYS_smearResJet"
"SYS_smearResJetPhi"
"SYS_smearZ"
"SYS_noReweightCent"
#"SYS_noCorrEle"
#"SYS_noCorrJetRes"
#"SYS_noJetID"
#"SYS_noJetIDCorr"
);

configSuffixList_histSum_HI_ZEE=("${configSuffixList_hist_HI_ZEE[@]}")

cutList_hist_HI_ZEE=(
"# skim.jet.energyScale = 1.06"
"# skim.jet.energyScale = 0.96"
"# skim.electron.energyScale = 1.005"
"# skim.electron.energyScale = 0.995"
"# skim.jet.smearingRes = 0.15"
"# skim.jet.smearingResPhi = 0.15"
"# skim.zboson.smearZ = 0.05"
"histogram.zboson.doDiElectron_reweightCent = 0"
#"# skim.electron.doCorrection = 0"
#"# skim.jet.doCorrectionResidual = 0"
#"histogram.jet.jetID = 0 \n histogram.jet.doCorrectionJetID = 0"
#"histogram.jet.jetID = 1 \n histogram.jet.doCorrectionJetID = 0"
);

skimSuffixList_hist_HI_ZEE=(
"SYS_jesPlus"
"SYS_jesMinus"
"SYS_eesPlus"
"SYS_eesMinus"
"SYS_smearResJet"
"SYS_smearResJetPhi"
"SYS_smearZ"
""
#"SYS_noCorrEle"
#"SYS_noCorrJetRes"
#""
#""
);

## HI ZMM - skim
configSuffixList_skim_HI_ZMM=(
"SYS_jesPlus"
"SYS_jesMinus"
"SYS_smearResJet"
"SYS_smearResJetPhi"
#"SYS_noCorrEle"
#"SYS_noCorrJetRes"
#"SYS_noL2L3"
);

cutList_skim_HI_ZMM=(
"skim.jet.energyScale = 1.06"
"skim.jet.energyScale = 0.96"
"skim.jet.smearingRes = 0.15"
"skim.jet.smearingResPhi = 0.15"
#"skim.electron.doCorrection = 0"
#"skim.jet.doCorrectionResidual = 0"
#"skim.jet.doCorrectionL2L3 = 0"
);

## HI ZMM - histogram
configSuffixList_hist_HI_ZMM=(
"SYS_jesPlus"
"SYS_jesMinus"
"SYS_smearResJet"
"SYS_smearResJetPhi"
#"SYS_noCorrEle"
#"SYS_noCorrJetRes"
#"SYS_noJetID"
#"SYS_noJetIDCorr"
);

configSuffixList_histSum_HI_ZMM=(
"SYS_jesPlus"
"SYS_jesMinus"
""     # SYS_eesPlus
""     # SYS_eesMinus
"SYS_smearResJet"
"SYS_smearResJetPhi"
""     # SYS_smearZ
""     # SYS_noReweightCent
#"SYS_noCorrEle"
#"SYS_noCorrJetRes"
#"SYS_noJetID"
#"SYS_noJetIDCorr"
);

cutList_hist_HI_ZMM=(
"# skim.jet.energyScale = 1.06"
"# skim.jet.energyScale = 0.96"
"# skim.jet.smearingRes = 0.15"
"# skim.jet.smearingResPhi = 0.15"
#"# skim.electron.doCorrection = 0"
#"# skim.jet.doCorrectionResidual = 0"
#"histogram.jet.jetID = 0 \n histogram.jet.doCorrectionJetID = 0"
#"histogram.jet.jetID = 1 \n histogram.jet.doCorrectionJetID = 0"
);

skimSuffixList_hist_HI_ZMM=(
"SYS_jesPlus"
"SYS_jesMinus"
"SYS_smearResJet"
"SYS_smearResJetPhi"
#"SYS_noCorrEle"
#"SYS_noCorrJetRes"
#""
#""
);

## PP ZEE - skim
configSuffixList_skim_PP_ZEE=(
"SYS_jesPlus"
"SYS_jesMinus"
"SYS_eesPlus"
"SYS_eesMinus"
"SYS_corrJetSmearPlus"
"SYS_corrJetSmearMinus"
"SYS_corrJetSmearPhiPlus"
"SYS_corrJetSmearPhiMinus"
"SYS_noCorrJetSmearALL"
#"SYS_noCorrEle"
#"SYS_noCorrJetSmear"
#"SYS_noCorrJetSmearPhi"
#"SYS_noL2L3"
);

cutList_skim_PP_ZEE=(
"skim.jet.energyScale = 1.02"
"skim.jet.energyScale = 0.98"
"skim.electron.energyScale = 1.005"
"skim.electron.energyScale = 0.995"
"skim.jet.doCorrectionSmearing = 2"
"skim.jet.doCorrectionSmearing = 3"
"skim.jet.doCorrectionSmearingPhi = 2"
"skim.jet.doCorrectionSmearingPhi = 3"
"skim.jet.doCorrectionSmearing = 0 \n skim.jet.doCorrectionSmearingPhi = 0"
#"skim.electron.doCorrection = 0"
#"skim.jet.doCorrectionSmearing = 0"
#"skim.jet.doCorrectionSmearingPhi = 0"
#"skim.jet.doCorrectionL2L3 = 0"
);

## PP ZEE - histogram
configSuffixList_hist_PP_ZEE=(
"SYS_jesPlus"
"SYS_jesMinus"
"SYS_eesPlus"
"SYS_eesMinus"
"SYS_corrJetSmearPlus"
"SYS_corrJetSmearMinus"
"SYS_corrJetSmearPhiPlus"
"SYS_corrJetSmearPhiMinus"
"SYS_noCorrJetSmearALL"
#"SYS_noCorrEle"
#"SYS_noCorrJetSmear"
#"SYS_noCorrJetSmearPhi"
#"SYS_noL2L3"
);

configSuffixList_histSum_PP_ZEE=("${configSuffixList_hist_PP_ZEE[@]}")

cutList_hist_PP_ZEE=(
"# skim.jet.energyScale = 1.02"
"# skim.jet.energyScale = 0.98"
"# skim.electron.energyScale = 1.005"
"# skim.electron.energyScale = 0.995"
"# skim.jet.doCorrectionSmearing = 2"
"# skim.jet.doCorrectionSmearing = 3"
"# skim.jet.doCorrectionSmearingPhi = 2"
"# skim.jet.doCorrectionSmearingPhi = 3"
"# skim.jet.doCorrectionSmearing = 0 \n skim.jet.doCorrectionSmearingPhi = 0"
#"# skim.electron.doCorrection = 0"
#"# skim.jet.doCorrectionSmearing = 0"
#"# skim.jet.doCorrectionSmearingPhi = 0"
#"# skim.jet.doCorrectionL2L3 = 0"
);

skimSuffixList_hist_PP_ZEE=(
"SYS_jesPlus"
"SYS_jesMinus"
"SYS_eesPlus"
"SYS_eesMinus"
"SYS_corrJetSmearPlus"
"SYS_corrJetSmearMinus"
"SYS_corrJetSmearPhiPlus"
"SYS_corrJetSmearPhiMinus"
"SYS_noCorrJetSmearALL"
#"SYS_noCorrEle"
#"SYS_noCorrJetSmear"
#"SYS_noCorrJetSmearPhi"
#"SYS_noL2L3"
);

## PP ZMM - skim
configSuffixList_skim_PP_ZMM=(
"SYS_jesPlus"
"SYS_jesMinus"
"SYS_corrJetSmearPlus"
"SYS_corrJetSmearMinus"
"SYS_corrJetSmearPhiPlus"
"SYS_corrJetSmearPhiMinus"
"SYS_noCorrJetSmearALL"
#"SYS_noCorrEle"
#"SYS_noCorrJetSmear"
#"SYS_noCorrJetSmearPhi"
#"SYS_noL2L3"
);

cutList_skim_PP_ZMM=(
"skim.jet.energyScale = 1.02"
"skim.jet.energyScale = 0.98"
"skim.jet.doCorrectionSmearing = 2"
"skim.jet.doCorrectionSmearing = 3"
"skim.jet.doCorrectionSmearingPhi = 2"
"skim.jet.doCorrectionSmearingPhi = 3"
"skim.jet.doCorrectionSmearing = 0 \n skim.jet.doCorrectionSmearingPhi = 0"
#"skim.electron.doCorrection = 0"
#"skim.jet.doCorrectionSmearing = 0"
#"skim.jet.doCorrectionSmearingPhi = 0"
#"skim.jet.doCorrectionL2L3 = 0"
);

## PP ZMM - histogram
configSuffixList_hist_PP_ZMM=(
"SYS_jesPlus"
"SYS_jesMinus"
"SYS_corrJetSmearPlus"
"SYS_corrJetSmearMinus"
"SYS_corrJetSmearPhiPlus"
"SYS_corrJetSmearPhiMinus"
"SYS_noCorrJetSmearALL"
#"SYS_noCorrEle"
#"SYS_noCorrJetSmear"
#"SYS_noCorrJetSmearPhi"
#"SYS_noL2L3"
);

configSuffixList_histSum_PP_ZMM=(
"SYS_jesPlus"
"SYS_jesMinus"
""     # SYS_eesPlus
""     # SYS_eesMinus
"SYS_corrJetSmearPlus"
"SYS_corrJetSmearMinus"
"SYS_corrJetSmearPhiPlus"
"SYS_corrJetSmearPhiMinus"
"SYS_noCorrJetSmearALL"
#"SYS_noCorrEle"
#"SYS_noCorrJetSmear"
#"SYS_noCorrJetSmearPhi"
#"SYS_noL2L3"
);

cutList_hist_PP_ZMM=(
"# skim.jet.energyScale = 1.02"
"# skim.jet.energyScale = 0.98"
"# skim.jet.doCorrectionSmearing = 2"
"# skim.jet.doCorrectionSmearing = 3"
"# skim.jet.doCorrectionSmearingPhi = 2"
"# skim.jet.doCorrectionSmearingPhi = 3"
"# skim.jet.doCorrectionSmearing = 0 \n skim.jet.doCorrectionSmearingPhi = 0"
#"# skim.electron.doCorrection = 0"
#"# skim.jet.doCorrectionSmearing = 0"
#"# skim.jet.doCorrectionSmearingPhi = 0"
#"# skim.jet.doCorrectionL2L3 = 0"
);

skimSuffixList_hist_PP_ZMM=(
"SYS_jesPlus"
"SYS_jesMinus"
"SYS_corrJetSmearPlus"
"SYS_corrJetSmearMinus"
"SYS_corrJetSmearPhiPlus"
"SYS_corrJetSmearPhiMinus"
"SYS_noCorrJetSmearALL"
#"SYS_noCorrEle"
#"SYS_noCorrJetSmear"
#"SYS_noCorrJetSmearPhi"
#"SYS_noL2L3"
);

## arrays for skim step - DATA
configSuffixList_skim_HI_ZEE_DATA=("${configSuffixList_skim_HI_ZEE[@]}")
configSuffixList_skim_HI_ZMM_DATA=("${configSuffixList_skim_HI_ZMM[@]}")
configSuffixList_skim_PP_ZEE_DATA=("${configSuffixList_skim_PP_ZEE[@]}")
configSuffixList_skim_PP_ZMM_DATA=("${configSuffixList_skim_PP_ZMM[@]}")

cutList_skim_HI_ZEE_DATA=("${cutList_skim_HI_ZEE[@]}")
cutList_skim_HI_ZMM_DATA=("${cutList_skim_HI_ZMM[@]}")
cutList_skim_PP_ZEE_DATA=("${cutList_skim_PP_ZEE[@]}")
cutList_skim_PP_ZMM_DATA=("${cutList_skim_PP_ZMM[@]}")

## arrays for skim step - MC
configSuffixList_skim_HI_ZEE_MC=("${configSuffixList_skim_HI_ZEE[@]}")
configSuffixList_skim_HI_ZMM_MC=("${configSuffixList_skim_HI_ZMM[@]}")
configSuffixList_skim_PP_ZEE_MC=("${configSuffixList_skim_PP_ZEE[@]}")
configSuffixList_skim_PP_ZMM_MC=("${configSuffixList_skim_PP_ZMM[@]}")

cutList_skim_HI_ZEE_MC=("${cutList_skim_HI_ZEE[@]}")
cutList_skim_HI_ZMM_MC=("${cutList_skim_HI_ZMM[@]}")
cutList_skim_PP_ZEE_MC=("${cutList_skim_PP_ZEE[@]}")
cutList_skim_PP_ZMM_MC=("${cutList_skim_PP_ZMM[@]}")

## arrays for histogram step - DATA
configSuffixList_hist_HI_ZEE_DATA=("${configSuffixList_hist_HI_ZEE[@]}")
configSuffixList_hist_HI_ZMM_DATA=("${configSuffixList_hist_HI_ZMM[@]}")
configSuffixList_hist_PP_ZEE_DATA=("${configSuffixList_hist_PP_ZEE[@]}")
configSuffixList_hist_PP_ZMM_DATA=("${configSuffixList_hist_PP_ZMM[@]}")

configSuffixList_histSum_HI_ZEE_DATA=("${configSuffixList_histSum_HI_ZEE[@]}")
configSuffixList_histSum_HI_ZMM_DATA=("${configSuffixList_histSum_HI_ZMM[@]}")
configSuffixList_histSum_PP_ZEE_DATA=("${configSuffixList_histSum_PP_ZEE[@]}")
configSuffixList_histSum_PP_ZMM_DATA=("${configSuffixList_histSum_PP_ZMM[@]}")

cutList_hist_HI_ZEE_DATA=("${cutList_hist_HI_ZEE[@]}")
cutList_hist_HI_ZMM_DATA=("${cutList_hist_HI_ZMM[@]}")
cutList_hist_PP_ZEE_DATA=("${cutList_hist_PP_ZEE[@]}")
cutList_hist_PP_ZMM_DATA=("${cutList_hist_PP_ZMM[@]}")

skimSuffixList_hist_HI_ZEE_DATA=("${skimSuffixList_hist_HI_ZEE[@]}")
skimSuffixList_hist_HI_ZMM_DATA=("${skimSuffixList_hist_HI_ZMM[@]}")
skimSuffixList_hist_PP_ZEE_DATA=("${skimSuffixList_hist_PP_ZEE[@]}")
skimSuffixList_hist_PP_ZMM_DATA=("${skimSuffixList_hist_PP_ZMM[@]}")

## arrays for histogram step - MC
configSuffixList_hist_HI_ZEE_MC=("${configSuffixList_hist_HI_ZEE[@]}")
configSuffixList_hist_HI_ZMM_MC=("${configSuffixList_hist_HI_ZMM[@]}")
configSuffixList_hist_PP_ZEE_MC=("${configSuffixList_hist_PP_ZEE[@]}")
configSuffixList_hist_PP_ZMM_MC=("${configSuffixList_hist_PP_ZMM[@]}")

configSuffixList_histSum_HI_ZEE_MC=("${configSuffixList_histSum_HI_ZEE[@]}")
configSuffixList_histSum_HI_ZMM_MC=("${configSuffixList_histSum_HI_ZMM[@]}")
configSuffixList_histSum_PP_ZEE_MC=("${configSuffixList_histSum_PP_ZEE[@]}")
configSuffixList_histSum_PP_ZMM_MC=("${configSuffixList_histSum_PP_ZMM[@]}")

cutList_hist_HI_ZEE_MC=("${cutList_hist_HI_ZEE[@]}")
cutList_hist_HI_ZMM_MC=("${cutList_hist_HI_ZMM[@]}")
cutList_hist_PP_ZEE_MC=("${cutList_hist_PP_ZEE[@]}")
cutList_hist_PP_ZMM_MC=("${cutList_hist_PP_ZMM[@]}")

skimSuffixList_hist_HI_ZEE_MC=("${skimSuffixList_hist_HI_ZEE[@]}")
skimSuffixList_hist_HI_ZMM_MC=("${skimSuffixList_hist_HI_ZMM[@]}")
skimSuffixList_hist_PP_ZEE_MC=("${skimSuffixList_hist_PP_ZEE[@]}")
skimSuffixList_hist_PP_ZMM_MC=("${skimSuffixList_hist_PP_ZMM[@]}")
#### global variables - END

