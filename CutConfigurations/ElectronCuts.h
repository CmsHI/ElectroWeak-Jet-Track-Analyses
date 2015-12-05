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
      k_nEle,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {"nEle"};

    enum S_CUTS{
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {};

  };
}

#endif
