#ifndef CUTCONFIGURATION_trk
#define CUTCONFIGURATION_trk

namespace CUTS{
  namespace TRK {
    enum F_CUTS{
        k_trkPt,
        k_trkEta,
        k_trkMVA,
        k_trkMVA_99,
        k_trkPtErrorOverPt,
        k_trkDz1OverError1,
        k_trkDxy1OverError1,
        kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {
            "trkPt",
            "trkEta",
            "trkMVA",
            "trkMVA_99",
            "trkPtErrorOverPt",
            "trkDz1OverError1",
            "trkDxy1OverError1"
    };

    enum I_CUTS{
        k_trkNHit,
        k_highPurity,
        k_loose,
        k_tight,
        kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {
            "trkNHit",
            "highPurity",
            "loose",
            "tight"
    };

    enum S_CUTS{
        k_quality,
        kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {
            "quality"
    };

  };
};

#endif
