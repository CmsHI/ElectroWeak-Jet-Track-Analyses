#ifndef CUTCONFIGURATION_trk
#define CUTCONFIGURATION_trk

namespace CUTS{
  namespace TRK {
    enum F_CUTS{
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {
    };

    enum I_CUTS{
      k_highPurity,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {"highPurity"};

    enum S_CUTS{
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {};

  };
};

#endif
