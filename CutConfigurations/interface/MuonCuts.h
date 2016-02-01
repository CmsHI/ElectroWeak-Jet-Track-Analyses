/*
 * MuonCuts.h
 *
 *  Created on: Jan 28, 2016
 *      Author: kaya
 */

#ifndef CUTCONFIGURATIONS_INTERFACE_MUONCUTS_H_
#define CUTCONFIGURATIONS_INTERFACE_MUONCUTS_H_

namespace CUTS {
  namespace MUO {
    enum F_CUTS{
      k_muPt,
      k_muChi2NDF,
      k_muInnerD0,
      k_muInnerDz,
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {
            "muPt",
            "muChi2NDF",
            "muInnerD0",
            "muInnerDz"
    };

    enum I_CUTS{
      k_nMu,
      k_muMuonHits,
      k_muStations,
      k_muTrkLayers,
      k_muPixelHits,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {
            "nMu",
            "muMuonHits",
            "muStations",
            "muTrkLayers",
            "muPixelHits"
    };

    enum S_CUTS{
      k_trigger,
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {
            "trigger"
    };

  };
}


#endif /* CUTCONFIGURATIONS_INTERFACE_MUONCUTS_H_ */
