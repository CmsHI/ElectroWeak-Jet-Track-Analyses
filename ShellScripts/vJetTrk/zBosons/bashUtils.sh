source ./ShellScripts/bashUtils.sh
## global variables
IFS=$'\n' # very important. force to use only '\n' to split the elements of the array
### forests - v+Jet
forest_pp_2017_mc_zmm="Configurations/filelists/HIRun2017PP/official/Z15mumuJet_pThat_15_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR_94X_mc2017_realistic_forppRef5TeV_v1_v1_FOREST.list"
forest_pp_2017_mc_zee="Configurations/filelists/HIRun2017PP/official/Z15eeJet_pThat_15_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR_94X_mc2017_realistic_forppRef5TeV_v1_v1_FOREST_unmerged.list"
forest_pp_2017_data_zmm="Configurations/filelists/HIRun2017PP/SingleMuon_Run2017G_17Nov2017_v1_Cert_306546_306826_filterZMM_FOREST.list"
forest_pp_2017_data_zee="Configurations/filelists/HIRun2017PP/HighEGJet_Run2017G_17Nov2017_v2_Cert_306546_306826_filterZEE_FOREST_unmerged.list"

## pbpb 2018
forest_pbpb_2018_mc_zmm="Configurations/filelists/HIRun2018PbPb/official/Z15mumuJet_pThat_15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8_HINPbPbAutumn18DR_mva98_103X_upgrade2018_realistic_HI_v11_v1_FOREST.list"
forest_pbpb_2018_mc_zee="Configurations/filelists/HIRun2018PbPb/official/Z15eeJet_pThat_15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8_HINPbPbAutumn18DR_mva98_103X_upgrade2018_realistic_HI_v11_v1_FOREST.list"
forest_pbpb_2018_data_zmm="Configurations/filelists/HIRun2018PbPb/HISingleMuon_HIRun2018A_04Apr2019_v1_Cert_326381_327564_filterZMM_FOREST_unmerged.list"
forest_pbpb_2018_data_zee="Configurations/filelists/HIRun2018PbPb/HIHardProbes_HIRun2018A_04Apr2019_v1_Cert_326381_327564_filterZEE_FOREST_unmerged.list"

## pp 2015
forest_pp_2015_mc_zmm="Configurations/filelists/HIRun2015PP/official/Pythia8_Z30mumuJet_HINppWinter16DR_75X_mcRun2_asymptotic_ppAt5TeV_v3_ext1_v1_FOREST.list"
forest_pp_2015_mc_zee="Configurations/filelists/HIRun2015PP/official/Pythia8_Z30eeJet_HINppWinter16DR_75X_mcRun2_asymptotic_ppAt5TeV_v3_ext1_v1_FOREST.list"
forest_pp_2015_data_zmm="Configurations/filelists/HIRun2015PP/SingleMuHighPt_HIRun2015E_PromptReco_AOD_dimuon_skim_FOREST.list"
forest_pp_2015_data_zee="Configurations/filelists/HIRun2015PP/HighPtPhoton30AndZ_HIRun2015E_PromptReco_AOD_dielectron_skim_FOREST.list"

## pbpb 2015
forest_pbpb_2015_mc_zmm="Configurations/filelists/HIRun2015PbPb/official/Pythia8_Z30mumuJet_Hydjet_MB_HINPbPbWinter16DR_75X_mcRun2_HeavyIon_v13_ext1_FOREST_unmerged.list"
forest_pbpb_2015_mc_zee="Configurations/filelists/HIRun2015PbPb/official/Pythia8_Z30eeJet_Hydjet_MB_HINPbPbWinter16DR_75X_mcRun2_HeavyIon_v13_ext1_v1_FOREST_unmerged.list"
forest_pbpb_2015_data_zmm="Configurations/filelists/HIRun2015PbPb/HIEWQExo_HIRun2015_PromptReco_v1_AOD_dimuon_skim_FOREST_unmerged.list"
forest_pbpb_2015_data_zee="Configurations/filelists/HIRun2015PbPb/HIPhoton40AndZ_HIRun2015_PromptReco_v1_AOD_dielectron_skim_FOREST_unmerged.list"

### forests - mix event
forest_pp_2017_mc_mix="Configurations/filelists/HIRun2017PP/official/MinBias_TuneCUETP8M1_2017_5p02TeV_pythia8_RunIIpp5Spring18DR_94X_mc2017_realistic_forppRef5TeV_v1_FOREST.list"
forest_pp_2017_data_mix="Configurations/filelists/HIRun2017PP/ParkingL1MinimumBias6_Run2017G_17Nov2017_v1_Cert_306546_306826_FOREST.list"

forest_pbpb_2018_mc_mix="Configurations/filelists/HIRun2018PbPb/official/MinBias_Hydjet_Drum5F_2018_5p02TeV_HINPbPbAutumn18DR_NoPUmva98_103X_upgrade2018_realistic_HI_v11_v1_FOREST.list"
forest_pbpb_2018_data_mix_1="Configurations/filelists/HIRun2018PbPb/HIMinimumBias0_HIRun2018A_04Apr2019_v1_Run326801_327200_FOREST.list"
forest_pbpb_2018_data_mix_2="Configurations/filelists/HIRun2018PbPb/HIMinimumBias0_HIRun2018A_04Apr2019_v1_Run327201_327400_FOREST.list"
forest_pbpb_2018_data_mix_3="Configurations/filelists/HIRun2018PbPb/HIMinimumBias0_HIRun2018A_04Apr2019_v1_Run326381_326800_FOREST.list"
forest_pbpb_2018_data_mix_4="Configurations/filelists/HIRun2018PbPb/HIMinimumBias0_HIRun2018A_04Apr2019_v1_Run327401_327564_FOREST.list"
forest_pbpb_2018_data_mix="Configurations/filelists/HIRun2018PbPb/HIMinimumBias0_HIRun2018A_04Apr2019_v1_Run326801_327400_FOREST.list"

forest_pp_2015_mc_mix="DUMMY.root"
forest_pp_2015_data_mix_1="Configurations/filelists/HIRun2015PP/MinimumBias1_Run2015E_PromptReco_v1_Run262163_262328_Cert_FOREST.list"
forest_pp_2015_data_mix_2="Configurations/filelists/HIRun2015PP/MinimumBias2_Run2015E_PromptReco_v1_Run262163_262328_Cert_FOREST.list"
forest_pp_2015_data_mix_3="Configurations/filelists/HIRun2015PP/MinimumBias3_Run2015E_PromptReco_v1_Run262163_262328_Cert_FOREST.list"
forest_pp_2015_data_mix="Configurations/filelists/HIRun2015PP/MinimumBias123_Run2015E_PromptReco_v1_Run262163_262328_Cert_FOREST.list"

forest_pbpb_2015_mc_mix="Configurations/filelists/official/HIRun2015PbPb/Hydjet_Quenched_Cymbal5Ev8_PbPbMinBias_5020GeV.list"
forest_pbpb_2015_data_mix_1="Configurations/filelists/HIRun2015PbPb/HIMinimumBias2_HIRun2015_PromptReco_v1_Run263322_263333_FOREST.list"
forest_pbpb_2015_data_mix_2="Configurations/filelists/HIRun2015PbPb/HIMinimumBias2_HIRun2015_PromptReco_v1_Run263349_263379_FOREST.list"

### forests - noPU
forest_pbpb_2018_mc_zmm_noPU="Configurations/filelists/HIRun2018PbPb/official/Z15mumuJet_pThat_15_TuneCP5_PbPb_5p02TeV_pythia8_HINPbPbAutumn18DR_NoPUmva98_103X_upgrade2018_realistic_HI_v11_v1_FOREST.list"

### skims
## pp 2017
skim_pp_2017_mc_zmm="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pp_2017_mc_zmm.list"
skim_pp_2017_mc_zee="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pp_2017_mc_zee.list"
skim_pp_2017_data_zmm="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pp_2017_data_zmm.list"
skim_pp_2017_data_zee="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pp_2017_data_zee.list"

## pbpb 2018
skim_pbpb_2018_mc_zmm="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pbpb_2018_mc_zmm.list"
skim_pbpb_2018_mc_zee="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pbpb_2018_mc_zee.list"
skim_pbpb_2018_data_zmm="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pbpb_2018_data_zmm.list"
skim_pbpb_2018_data_zee="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pbpb_2018_data_zee.list"
## pp 2015
skim_pp_2015_mc_zmm="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pp_2015_mc_zmm.list"
skim_pp_2015_mc_zee="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pp_2015_mc_zee.list"
skim_pp_2015_data_zmm="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pp_2015_data_zmm.list"
skim_pp_2015_data_zee="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pp_2015_data_zee.list"

## pbpb 2015
skim_pbpb_2015_mc_zmm="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pbpb_2015_mc_zmm.list"
skim_pbpb_2015_mc_zee="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pbpb_2015_mc_zee.list"
skim_pbpb_2015_data_zmm="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pbpb_2015_data_zmm.list"
skim_pbpb_2015_data_zee="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pbpb_2015_data_zee.list"

### skims - mix event
skim_pbpb_2018_mc_mix="Configurations/filelists/vJetTrk/zBoson/mixFileSkim_pbpb_2018_mc_mix.list"
skim_pbpb_2018_data_mix="Configurations/filelists/vJetTrk/zBoson/mixFileSkim_pbpb_2018_data_mix.list"
skim_pbpb_2018_data_mix_1="Configurations/filelists/vJetTrk/zBoson/mixFileSkim_pbpb_2018_data_mix_1.list"
skim_pbpb_2018_data_mix_2="Configurations/filelists/vJetTrk/zBoson/mixFileSkim_pbpb_2018_data_mix_2.list"
skim_pbpb_2018_data_mix_3="Configurations/filelists/vJetTrk/zBoson/mixFileSkim_pbpb_2018_data_mix_3.list"
skim_pbpb_2018_data_mix_4="Configurations/filelists/vJetTrk/zBoson/mixFileSkim_pbpb_2018_data_mix_4.list"

skim_pbpb_2015_data_mix="Configurations/filelists/vJetTrk/zBoson/mixFileSkim_pbpb_2015_data_mix.list"
skim_pbpb_2015_data_mix_1="Configurations/filelists/vJetTrk/zBoson/mixFileSkim_pbpb_2015_data_mix_1.list"
skim_pbpb_2015_data_mix_2="Configurations/filelists/vJetTrk/zBoson/mixFileSkim_pbpb_2015_data_mix_2.list"

## pbpb 2018 noPU
skim_pbpb_2018_mc_zmm_noPU="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pbpb_2018_mc_zmm_noPU.list"

configListAna=(
## pp 2017
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2017.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2017.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2017.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2017.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2017.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2017.data.conf"
## pbpb 2018
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2018.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2018.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2018.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2018.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2018.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2018.data.conf"
## pp 2015
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2015.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2015.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2015.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2015.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2015.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2015.data.conf"
## pbpb 2015
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2015.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2015.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2015.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2015.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2015.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2015.data.conf"
## pbpb 2018 noPU
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2018.mc.conf"
);

skimList=(
## pp 2017
${skim_pp_2017_mc_zmm}
${skim_pp_2017_mc_zee}
${skim_pp_2017_data_zmm}
${skim_pp_2017_data_zee}
## pbpb 2018
${skim_pbpb_2018_mc_zmm}
${skim_pbpb_2018_mc_zee}
${skim_pbpb_2018_data_zmm}
${skim_pbpb_2018_data_zee}
## pp 2015
${skim_pp_2015_mc_zmm}
${skim_pp_2015_mc_zee}
${skim_pp_2015_data_zmm}
${skim_pp_2015_data_zee}
## pbpb 2015
${skim_pbpb_2015_mc_zmm}
${skim_pbpb_2015_mc_zee}
${skim_pbpb_2015_data_zmm}
${skim_pbpb_2015_data_zee}
## pbpb 2018 noPU
${skim_pbpb_2018_mc_zmm_noPU}
);

## condor directories
condorSubmitDir="/work/tatar/ewjta/condorSubmissions/vJetTrk/"

## suffix for skim output
relDirSkim="vJetTrk/zBoson/ForestSkimmers"
relDirHist="vJetTrk/zBoson/Histogramming"
relDirSpectra="vJetTrk/zBoson/spectra"
relDirCondorCreateLog="vJetTrk/zBoson/CondorCreateLog"

## suffix for analysis output
anaSuffixList=(
##
"vJetTrkAna_pp_2017_mc_zmm"
"vJetTrkAna_pp_2017_mc_zee"
"vJetTrkAna_pp_2017_data_zmm"
"vJetTrkAna_pp_2017_data_zee"
"vJetTrkCalc_pp_2017_mc_zll_merge"
"vJetTrkCalc_pp_2017_data_zll_merge"
##
"vJetTrkAna_pbpb_2018_mc_zmm"
"vJetTrkAna_pbpb_2018_mc_zee"
"vJetTrkAna_pbpb_2018_data_zmm"
"vJetTrkAna_pbpb_2018_data_zee"
"vJetTrkCalc_pbpb_2018_mc_zll_merge"
"vJetTrkCalc_pbpb_2018_data_zll_merge"
##
"vJetTrkAna_pp_2015_mc_zmm"
"vJetTrkAna_pp_2015_mc_zee"
"vJetTrkAna_pp_2015_data_zmm"
"vJetTrkAna_pp_2015_data_zee"
"vJetTrkCalc_pp_2015_mc_zll_merge"
"vJetTrkCalc_pp_2015_data_zll_merge"
##
"vJetTrkAna_pbpb_2015_mc_zmm"
"vJetTrkAna_pbpb_2015_mc_zee"
"vJetTrkAna_pbpb_2015_data_zmm"
"vJetTrkAna_pbpb_2015_data_zee"
"vJetTrkCalc_pbpb_2015_mc_zll_merge"
"vJetTrkCalc_pbpb_2015_data_zll_merge"
##
"vJetTrkAna_pbpb_2018_mc_zmm_noPU"
);

## suffix for calculation output
calcSuffixList=(
##
"vJetTrkCalc_pp_2017_mc_zmm"
"vJetTrkCalc_pp_2017_mc_zee"
"vJetTrkCalc_pp_2017_data_zmm"
"vJetTrkCalc_pp_2017_data_zee"
"vJetTrkCalc_pp_2017_mc_zll"
"vJetTrkCalc_pp_2017_data_zll"
##
"vJetTrkCalc_pbpb_2018_mc_zmm"
"vJetTrkCalc_pbpb_2018_mc_zee"
"vJetTrkCalc_pbpb_2018_data_zmm"
"vJetTrkCalc_pbpb_2018_data_zee"
"vJetTrkCalc_pbpb_2018_mc_zll"
"vJetTrkCalc_pbpb_2018_data_zll"
##
"vJetTrkCalc_pp_2015_mc_zmm"
"vJetTrkCalc_pp_2015_mc_zee"
"vJetTrkCalc_pp_2015_data_zmm"
"vJetTrkCalc_pp_2015_data_zee"
"vJetTrkCalc_pp_2015_mc_zll"
"vJetTrkCalc_pp_2015_data_zll"
##
"vJetTrkCalc_pbpb_2015_mc_zmm"
"vJetTrkCalc_pbpb_2015_mc_zee"
"vJetTrkCalc_pbpb_2015_data_zmm"
"vJetTrkCalc_pbpb_2015_data_zee"
"vJetTrkCalc_pbpb_2015_mc_zll"
"vJetTrkCalc_pbpb_2015_data_zll"
##
"vJetTrkCalc_pbpb_2018_mc_zmm_noPU"
);

## suffix for calculation output
mergeSuffixList=(
##
"DUMMY"
"DUMMY"
"DUMMY"
"DUMMY"
"vJetTrkCalc_pp_2017_mc_zll_merge"
"vJetTrkCalc_pp_2017_data_zll_merge"
##
"DUMMY"
"DUMMY"
"DUMMY"
"DUMMY"
"vJetTrkCalc_pbpb_2018_mc_zll_merge"
"vJetTrkCalc_pbpb_2018_data_zll_merge"
##
"DUMMY"
"DUMMY"
"DUMMY"
"DUMMY"
"vJetTrkCalc_pp_2015_mc_zll_merge"
"vJetTrkCalc_pp_2015_data_zll_merge"
##
"DUMMY"
"DUMMY"
"DUMMY"
"DUMMY"
"vJetTrkCalc_pbpb_2015_mc_zll_merge"
"vJetTrkCalc_pbpb_2015_data_zll_merge"
##
"DUMMY"
);

### additional
skim_pbpb_2018_mc_zmm_Z15="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pbpb_2018_mc_zmm_Z15mumuJet.list"
skim_pp_2017_mc_zmm_Z15="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pp_2017_mc_zmm_Z15mumuJet.list"

skim_pbpb_2015_mc_zmm_Z30="Configurations/filelists/vJetTrk/zBoson/vJetTrkSkim_pbpb_2015_mc_zmm_Z30mumuJet.list"
