#ifndef CUTCONFIGURATIONS_evt
#define CUTCONFIGURATIONS_evt


namespace CUTS {
  namespace EVT {
    enum F_CUTS{
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {

    };

    enum I_CUTS{
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {

    };

    enum S_CUTS{
      k_bins_hiBin_gt,      // the current centrality bin is greater or equal to these centrality bins
      k_bins_hiBin_lt,      // the current centrality bin is less than these centrality bins
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {
      "bins_hiBin_gt",     // label starts with "bins_". this means the input will be a list of strings
      "bins_hiBin_lt"
    };

  };
};


#endif
