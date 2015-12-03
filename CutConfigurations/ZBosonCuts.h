#ifndef CUTCONFIGURATION_zbo
#define CUTCONFIGURATION_zbo

namespace CUTS {
  namespace ZBO {
    enum F_CUTS{
      k_mass,
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {"mass"
    };

    enum I_CUTS{
      k_nTracks,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {"nTracks"};

    enum S_CUTS{
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {};

  };
};

#endif
