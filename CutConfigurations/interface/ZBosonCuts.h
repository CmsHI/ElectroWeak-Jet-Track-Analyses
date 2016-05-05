#ifndef CUTCONFIGURATION_zbo
#define CUTCONFIGURATION_zbo

namespace CUTS {
  namespace ZBO {
    enum F_CUTS{
      k_massMin,
      k_massMax,
      k_eta,
      k_pt,
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {
            "massMin",
            "massMax",
            "eta",
            "pt"
    };

    enum I_CUTS{
      k_doDiElectron,
      k_doDiMuon,
      k_doDiElectron_reweightCent,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {
            "doDiElectron",
            "doDiMuon",
            "doDiElectron_reweightCent"     // centrality reweighting for Z-ee RECO inefficiency in central collisions
            };

    enum S_CUTS{
      k_bins_pt_gt,
      k_bins_pt_lt,
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {
      "bins_pt_gt",      // label starts with "bins_". this means the input will be a list of strings
      "bins_pt_lt"
    };

  };
};

#endif
