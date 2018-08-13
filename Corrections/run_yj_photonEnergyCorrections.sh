#void yj_photonEnergyCorrections(bool isPP = false, TString fnc_name = "gaus", bool isPhotonSample=true)
root -l -b -q 'yj_photonEnergyCorrections.C++(0,"DSCB",1,1)'
root -l -b -q 'yj_photonEnergyCorrections.C++(0,"gaus",1,1)'
#root -l -b -q 'yj_photonEnergyCorrections.C++(1,"gaus",1,1)'
root -l -b -q 'yj_photonEnergyCorrections.C++(1,"DSCB",1,1)'
