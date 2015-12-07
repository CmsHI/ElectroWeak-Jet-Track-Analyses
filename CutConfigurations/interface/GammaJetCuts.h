#ifndef CUTCONFIGURATIONS_gjt
#define CUTCONFIGURATIONS_gjt

namespace CUTS {
  namespace GJT {
    enum F_CUTS{
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {
    };

    enum I_CUTS{
      k_nMaxEvents_minBiasMixing,
      k_nCentralityBins,
      k_nVertexBins,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {
      "nMaxEvents_minBiasMixing",
      "nCentralityBins",
      "nVertexBins"
    };

    enum S_CUTS{
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {};

  };
};

#endif
