#void yj_eleMatchedPhotonZPeak_eleCut(
#                          bool isPP=false,
#                          bool isYJCorr=true,
#                          TString func="DSCB",
#                          //TString sample="AllQCD",
#                          const int apply_corrections = 0,
#                          TString configFile = "./../CutConfigurations/photonRaa_eleMatchedPhotonZPeak.conf",
#                          const char* fn_data = "/pnfs/knu.ac.kr/data/cms/store/user/ygo/photonOfficialMC2016/Z30eeJet/pbpb_data_azsigmon-HIRun2015E-PromptReco-AOD-DielectronSkim-ElePt8-v3_forest_csjet_v1_3.root",
#                          const char* fn_mc = "/pnfs/knu.ac.kr/data/cms/store/user/ygo/photonOfficialMC2016/Z30eeJet/pbpb_mc_Pythia8_Z30eeJet_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13_ext1-FOREST.root",
#                          const char* fn_weights = "") {
root -l -b -q 'yj_eleMatchedPhotonZPeak_eleCut.C++(0,1,1,"DSCB",1)'
root -l -b -q 'yj_eleMatchedPhotonZPeak_eleCut.C++(1,1,1,"DSCB",1)'
root -l -b -q 'yj_eleMatchedPhotonZPeak_eleCut.C++(0,1,0,"DSCB",1)'
root -l -b -q 'yj_eleMatchedPhotonZPeak_eleCut.C++(1,1,0,"DSCB",1)'
#root -l -b -q 'yj_eleMatchedPhotonZPeak_eleCut.C++(0,1,1,"gaus",1)'
#root -l -b -q 'yj_eleMatchedPhotonZPeak_eleCut.C++(1,1,1,"gaus",1)'
#root -l -b -q 'yj_eleMatchedPhotonZPeak_eleCut.C++(0,1,1,"DSCB",0)'
#root -l -b -q 'yj_eleMatchedPhotonZPeak_eleCut.C++(1,1,1,"DSCB",0)'
