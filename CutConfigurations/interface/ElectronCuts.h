#ifndef CUTCONFIGURATIONS_INTERFACE_ELECTRONCUTS_H_
#define CUTCONFIGURATIONS_INTERFACE_ELECTRONCUTS_H_

namespace CUTS {
  namespace ELE {
    enum F_CUTS{
      k_elePt,
      k_eleSigmaIEtaIEta_spike,
      k_eleSigmaIPhiIPhi_spike,
      k_eleSigmaIEtaIEta_2012_EB,
      k_eledEtaAtVtx_abs_EB,
      k_eledPhiAtVtx_abs_EB,
      k_eleHoverE_EB,
      k_eleEoverPInv_EB,
      k_eleD0_abs_EB,
      k_eleDz_abs_EB,
      k_eleSigmaIEtaIEta_2012_EE,
      k_eledEtaAtVtx_abs_EE,
      k_eledPhiAtVtx_abs_EE,
      k_eleHoverE_EE,
      k_eleEoverPInv_EE,
      k_eleD0_abs_EE,
      k_eleDz_abs_EE,
      k_energyScale,
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {
            "elePt",
            "eleSigmaIEtaIEta_spike",
            "eleSigmaIPhiIPhi_spike",
            "eleSigmaIEtaIEta_2012_EB",
            "eledEtaAtVtx_abs_EB",
            "eledPhiAtVtx_abs_EB",
            "eleHoverE_EB",
            "eleEoverPInv_EB",
            "eleD0_abs_EB",
            "eleDz_abs_EB",
            "eleSigmaIEtaIEta_2012_EE",
            "eledEtaAtVtx_abs_EE",
            "eledPhiAtVtx_abs_EE",
            "eleHoverE_EE",
            "eleEoverPInv_EE",
            "eleD0_abs_EE",
            "eleDz_abs_EE",
            "energyScale"
    };

    enum I_CUTS{
      k_nEle,
      k_eleMissHits_EB,
      k_eleMissHits_EE,
      k_doCorrection,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {
            "nEle",
            "eleMissHits_EB",
            "eleMissHits_EE",
            "doCorrection"
    };

    enum S_CUTS{
      k_electronID,
      k_trigger,
      k_bins_eta_gt,
      k_bins_eta_lt,
      k_bins_pt_gt,
      k_bins_pt_lt,
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {
            "electronID",
            "trigger",
            "bins_eta_gt",     // label starts with "bins_". this means the input will be a list of strings
            "bins_eta_lt",
            "bins_pt_gt",      // label starts with "bins_". this means the input will be a list of strings
            "bins_pt_lt"
    };

  };
}

#endif
