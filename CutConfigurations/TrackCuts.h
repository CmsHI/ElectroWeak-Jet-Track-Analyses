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
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {};

    enum B_CUTS{
      k_highPurity,
      kN_B_CUTS
    };

    std::string B_CUTS_LABELS[kN_B_CUTS] = {"highPurity"};

  };
};

#endif
