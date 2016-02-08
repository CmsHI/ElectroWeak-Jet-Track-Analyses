#ifndef CUTCONFIGURATIONS_evt
#define CUTCONFIGURATIONS_evt


namespace CUTS {
  namespace EVT {
    enum F_CUTS{
        k_vz,
        kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {
            "vz"
    };

    enum I_CUTS{
        k_doEventWeight,
        k_phfCoincFilter3,
        k_pprimaryVertexFilter,
        k_pclusterCompatibilityFilter,
        k_pcollisionEventSelection,       // pcollisionEventSelection = phfCoincFilter3 && pprimaryVertexFilter && pclusterCompatibilityFilter
        k_pPAprimaryVertexFilter,         // for pp
        k_pBeamScrapingFilter,            // for pp
        k_pHBHENoiseFilterResultProducer,
        k_HBHEIsoNoiseFilterResult,
        kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {
            "doEventWeight",
            "phfCoincFilter3",
            "pprimaryVertexFilter",
            "pclusterCompatibilityFilter",
            "pcollisionEventSelection",
            "pPAprimaryVertexFilter",
            "pBeamScrapingFilter",
            "pHBHENoiseFilterResultProducer",
            "HBHEIsoNoiseFilterResult"
    };

    enum S_CUTS{
        k_eventWeight,
        k_bins_hiBin_gt,      // the current centrality bin is greater or equal to these centrality bins
        k_bins_hiBin_lt,      // the current centrality bin is less than these centrality bins
        kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {
            "eventWeight",
            "bins_hiBin_gt",     // label starts with "bins_". this means the input will be a list of strings
            "bins_hiBin_lt"
    };

  };
};


#endif
