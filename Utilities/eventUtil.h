/*
 *  general purpose utilities used in event analysis
 *
 */

#ifndef UTILS_EVENTUTILS_H_
#define UTILS_EVENTUTILS_H_

#include <string>

namespace COLL {

    enum TYPE{
        kHI,
        kHIMC,
        kPP,
        kPPMC,
        kPA,
        kPAMC,
        kN_COLL
    };

    std::string LABELS[kN_COLL] = {
            "HI",
            "HIMC",
            "PP",
            "PPMC",
            "PA",
            "PAMC"
    };
};

std::string getCollisionTypeName (COLL::TYPE collision);
bool collisionIsMC(COLL::TYPE collision);
bool collisionIsDATA(COLL::TYPE collision);
bool collisionIsHI(COLL::TYPE collision);
bool collisionIsPP(COLL::TYPE collision);
bool collisionIsPA(COLL::TYPE collision);

std::string getCollisionTypeName (COLL::TYPE collision) {
  if (collision >=0 && collision < COLL::kN_COLL) return COLL::LABELS[collision];
  return "NULL";
}

bool collisionIsMC(COLL::TYPE collision) {

    return (collision == COLL::kHIMC || collision == COLL::kPPMC || collision == COLL::kPAMC);
}

bool collisionIsDATA(COLL::TYPE collision) {

    return (collision == COLL::kHI || collision == COLL::kPP || collision == COLL::kPA);
}

bool collisionIsHI(COLL::TYPE collision) {

    return (collision == COLL::kHI || collision == COLL::kHIMC);
}

bool collisionIsPP(COLL::TYPE collision) {

    return (collision == COLL::kPP || collision == COLL::kPPMC);
}

bool collisionIsPA(COLL::TYPE collision) {

    return (collision == COLL::kPA || collision == COLL::kPAMC);
}

#endif /* UTILS_EVENTUTILS_H_ */
