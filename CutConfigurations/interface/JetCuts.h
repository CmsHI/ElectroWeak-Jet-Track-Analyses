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
      k_jetType,       // 0 = all jets, 1 = leading Jet, 2 = subleading jet
      k_doCorrectionMatchingEfficiency,
      k_doCorrectionResidual,
      k_doCorrectionSmearing,
      k_doCorrectionSmearingPhi,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {
      "requirePhotonInEvent",
      "jetID",
      "jetType",
      "doCorrectionMatchingEfficiency",
      "doCorrectionResidual",
      "doCorrectionSmearing",
      "doCorrectionSmearingPhi"
    };

    enum S_CUTS{
      k_jetCollection,
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {"jetCollection"};

    enum JETINDEX{
      k_allJets,
      k_leading,
      k_subleading,
      kN_JETINDEX
    };

  };
};

#endif
