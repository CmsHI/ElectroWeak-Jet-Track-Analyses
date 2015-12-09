#ifndef CUTCONFIGURATION_pho
#define CUTCONFIGURATION_pho

namespace CUTS {
  namespace PHO {
    enum F_CUTS{
      k_eta,
      k_et,
      k_sigmaIEtaIEta_2012,
      k_purityFitLow,
      k_purityFitHigh,
      k_puritySieieThreshold,
      k_puritySignalShift,
      k_purityBackgroundShift,
      k_puritySieieHistMax,
      k_phoSigmaIEtaIEta_spike,
//      k_phoSigmaIPhiIPhi_spike,   // not available yet
      k_phoHoverE,
      k_pho_ecalClusterIsoR4,
      k_pho_hcalRechitIsoR4,
      k_pho_trackIsoR4PtCut20,
      k_phoSigmaIEtaIEta,
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {
      "eta",
      "et",
      "sigmaIEtaIEta_2012",
      "purityFitLow",
      "purityFitHigh",
      "puritySieieThreshold",
      "puritySignalShift",
      "purityBackgroundShift",
      "puritySieieHistMax",
      "phoSigmaIEtaIEta_spike",
//      "phoSigmaIPhiIPhi_spike",   // not available yet
      "phoHoverE",
      "pho_ecalClusterIsoR4",
      "pho_hcalRechitIsoR4",
      "pho_trackIsoR4PtCut20",
      "phoSigmaIEtaIEta",
    };

    enum I_CUTS{
      k_nPhotons,
      k_puritySieieBins,
      k_matched_eleIndex,
      k_MonteCarlo,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {
      "nPhotons",
      "puritySieieBins",
      "matched_eleIndex",
      "MonteCarlo"
    };

    enum S_CUTS{
      k_monteCarloWeightLabel,
      k_trigger_diphoton,
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {
      "monteCarloWeightLabel",
      "trigger_diphoton"
    };

  };
};

#endif
