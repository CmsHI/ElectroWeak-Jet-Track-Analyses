#void calcMCweights(bool isHI=false, TString sample="AllQCD")//"EmEnr","ZtoEE"
root -l -b -q 'calcMCweights.C++(0,"ZtoEE")'
root -l -b -q 'calcMCweights.C++(1,"ZtoEE")'
#root -l -b -q 'calcMCweights.C++(0,"AllQCD")'
#root -l -b -q 'calcMCweights.C++(1,"AllQCD")'
#root -l -b -q 'calcMCweights.C++(0,"EmEnr")'
#root -l -b -q 'calcMCweights.C++(1,"EmEnr")'
