#ifndef CUTCONFIGURATION_pho
#define CUTCONFIGURATION_pho

namespace CUTS {
  namespace PHO {
    enum F_CUTS{
      k_eta,
      k_et,
      k_sigmaIEtaIEta_2012,
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {"eta",
					    "et",
					    "sigmaIEtaIEta_2012"
    };

    enum I_CUTS{
      k_nPhotons,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {"nPhotons"};

    enum B_CUTS{
      kN_B_CUTS
    };

    std::string B_CUTS_LABELS[kN_B_CUTS] = {};
  };
};

#endif
