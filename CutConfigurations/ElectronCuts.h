#ifndef CUTCONFIGURATION_ele
#define CUTCONFIGURATION_ele

namespace CUTS {
  namespace ELE {
    enum F_CUTS{
      k_pt,
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {"pt"
    };

    enum I_CUTS{
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {};

    enum B_CUTS{
      kN_B_CUTS
    };

    std::string B_CUTS_LABELS[kN_B_CUTS] = {};
  };
}

#endif
