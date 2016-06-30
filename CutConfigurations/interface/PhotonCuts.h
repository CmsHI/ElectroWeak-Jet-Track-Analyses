#ifndef CUTCONFIGURATION_pho
#define CUTCONFIGURATION_pho

namespace CUTS {
  namespace PHO {
    enum F_CUTS{
      k_et,
      k_eta,
      k_phoSigmaIEtaIEta_2012,
      k_purityFitLow,
      k_purityFitHigh,
      k_puritySieieThreshold,
      k_puritySignalShift,
      k_purityBackgroundShift,
      k_puritySieieHistMax,
      k_phoSigmaIEtaIEta_spike,
      k_pho_swissCrx,
      k_pho_seedTime,
//      k_phoSigmaIPhiIPhi_spike,   // not available yet
      k_phoHoverE,
      k_pho_ecalClusterIsoR4,
      k_pho_hcalRechitIsoR4,
      k_pho_trackIsoR4PtCut20,
      k_phoSigmaIEtaIEta,
      k_sumIso,
      k_sumPfIso,
      // isolation for PP
      k_phoHOverE_EB,
      k_phoSigmaIEtaIEta_EB,
      k_pfcIso4_EB,
      k_pfnIso4_c0_EB,
      k_pfnIso4_c1_EB,
      k_pfnIso4_c2_EB,
      k_pfpIso4_c0_EB,
      k_pfpIso4_c1_EB,
      k_phoHOverE_EE,
      k_phoSigmaIEtaIEta_EE,
      k_pfcIso4_EE,
      k_pfnIso4_c0_EE,
      k_pfnIso4_c1_EE,
      k_pfnIso4_c2_EE,
      k_pfpIso4_c0_EE,
      k_pfpIso4_c1_EE,
      k_mcCalIsoDR04,
      k_mcTrkIsoDR04,
      k_mcSumIso,
      k_EtaLowerBound,
      k_EtaUpperBound,
      k_useUncorrectedPhotonEnergy,
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {
      "et",
      "eta",
      "phoSigmaIEtaIEta_2012",
      "purityFitLow",
      "purityFitHigh",
      "puritySieieThreshold",
      "puritySignalShift",
      "purityBackgroundShift",
      "puritySieieHistMax",
      "phoSigmaIEtaIEta_spike",
      "pho_swissCrx",
      "pho_seedTime",
//      "phoSigmaIPhiIPhi_spike",   // not available yet
      "phoHoverE",
      "pho_ecalClusterIsoR4",
      "pho_hcalRechitIsoR4",
      "pho_trackIsoR4PtCut20",
      "phoSigmaIEtaIEta",
      "sumIso",
      "sumPfIso",
      // isolation for PP
      "phoHOverE_EB",
      "phoSigmaIEtaIEta_EB",
      "pfcIso4_EB",
      "pfnIso4_c0_EB",
      "pfnIso4_c1_EB",
      "pfnIso4_c2_EB",
      "pfpIso4_c0_EB",
      "pfpIso4_c1_EB",
      "phoHOverE_EE",
      "phoSigmaIEtaIEta_EE",
      "pfcIso4_EE",
      "pfnIso4_c0_EE",
      "pfnIso4_c1_EE",
      "pfnIso4_c2_EE",
      "pfpIso4_c0_EE",
      "pfpIso4_c1_EE",
      "mcCalIsoDR04",
      "mcTrkIsoDR04",
      "mcSumIso",
      "etaLowerBound",
      "etaUpperBound",
      "useUncorrectedPhotonEnergy"
    };

    enum I_CUTS{
      k_nPhotons,
      k_puritySieieBins,
      k_matched_eleIndex,
      k_MonteCarlo,
      k_usePPstyleIso,
      k_doElectronRejection,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {
      "nPhotons",
      "puritySieieBins",
      "matched_eleIndex",
      "MonteCarlo",
      "usePPstyleIso",
      "doElectronRejection",
    };

    enum S_CUTS{
      k_monteCarloWeightLabel,
      k_trigger_gammaJet,
      k_trigger_diphoton,
      k_triggerNum_List,
      k_triggerDenom_List,
      k_mcPt_List,
      k_pt_List,
      k_bins_eta_gt,
      k_bins_eta_lt,
      k_bins_pt_gt,
      k_bins_pt_lt,
      k_bins_mcPt_gt,
      k_bins_mcPt_lt,
      k_energy_correction_file,
      k_pt_bin_numbers,
      k_cent_bin_numbers,
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {
      "monteCarloWeightLabel",
      "trigger_gammaJet",
      "trigger_diphoton",
      "triggerNum_List",     // label ends with "_List". this means the input will be a list of strings
      "triggerDenom_List",
      "mcPt_List",
      "pt_List",
      "bins_eta_gt",     // label starts with "bins_". this means the input will be a list of strings
      "bins_eta_lt",
      "bins_pt_gt",      // label starts with "bins_". this means the input will be a list of strings
      "bins_pt_lt",
      "bins_mcPt_gt",      // label starts with "bins_". this means the input will be a list of strings
      "bins_mcPt_lt",
      "energy_correction_file",
      "pt_bin_numbers",      // select pt bins for dphi width/pedestal plots
      "cent_bin_numbers"      // select cent bins for "
    };

  };
};

#endif
