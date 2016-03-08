#ifndef CUTCONFIGURATION_gen
#define CUTCONFIGURATION_gen

namespace CUTS {
  namespace GEN {
    enum F_CUTS{
      k_mcPt,
      k_mcEta,
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {
      "mcPt",
      "mcEta"
    };

    enum I_CUTS{
      k_nMC,
      k_mcPID,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {
      "nMC",
      "mcPID"
    };

    enum S_CUTS{
      k_bins_eta_gt,
      k_bins_eta_lt,
      k_bins_pt_gt,
      k_bins_pt_lt,
      k_mcPID_List,
      k_mcMomPID_List,
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {
      "bins_eta_gt",     // label starts with "bins_". this means the input will be a list of strings
      "bins_eta_lt",
      "bins_pt_gt",      // label starts with "bins_". this means the input will be a list of strings
      "bins_pt_lt",
      "mcPID_List",
      "mcMomPID_List"
    };

  };
};

#endif
