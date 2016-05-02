#ifndef CUTCONFIGURATION_jet
#define CUTCONFIGURATION_jet

namespace CUTS {
  namespace JET {
    enum F_CUTS{
      k_pt,
      k_eta,
      k_energyScale,
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {
      "pt",
      "eta",
      "energyScale"
    };

    enum I_CUTS{
      k_requirePhotonInEvent,
      k_jetID,
      k_doSubid,
      k_jetType,       // 0 = all jets, 1 = leading Jet, 2 = subleading jet
      k_doCorrectionMatchingEfficiency,
      k_doCorrectionResidual,
      k_doCorrectionSmearing,
      k_doCorrectionSmearingPhi,
      k_jetAlgoSmearing,
      k_smearingHiBin,
      k_doCorrectionJetID,
      k_doCorrectionL2L3,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {
      "requirePhotonInEvent",
      "jetID",
      "doSubid"
      "jetType",
      "doCorrectionMatchingEfficiency",
      "doCorrectionResidual",
      "doCorrectionSmearing",
      "doCorrectionSmearingPhi",
      "jetAlgoSmearing",
      "smearingHiBin",
      "doCorrectionJetID",
      "doCorrectionL2L3",
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

    enum JETALGO{
      k_akPU,
      k_akCS,
      k_akVS,
      kN_JETALGO
    };

  };
};

#endif
