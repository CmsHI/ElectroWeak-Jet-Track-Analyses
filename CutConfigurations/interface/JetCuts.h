#ifndef CUTCONFIGURATION_jet
#define CUTCONFIGURATION_jet

namespace CUTS {
  namespace JET {
    enum F_CUTS{
      k_pt,
      k_eta,
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {
      "pt",
      "eta"
    };

    enum I_CUTS{
      k_requirePhotonInEvent,
      k_jetID,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {
      "requirePhotonInEvent",
      "jetID"
    };

    enum S_CUTS{
      k_jetCollection,
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {"jetCollection"};

  };
};

#endif
