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
      "puritySieieHistMax"
    };

    enum I_CUTS{
      k_nPhotons,
      k_puritySieieBins,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {
      "nPhotons",
      "puritySieieBins"
    };

    enum S_CUTS{
      k_monteCarloWeightLabel,
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {
      "monteCarloWeightLabel"
    };

  };
};

#endif
