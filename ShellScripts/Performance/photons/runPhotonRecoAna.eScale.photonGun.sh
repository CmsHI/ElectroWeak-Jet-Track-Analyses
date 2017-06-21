#!/bin/bash

make
#g++ -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/photons/photonRecoAna.d Performance/photons/photonRecoAna.C -o Performance/photons/photonRecoAna.exe
#echo "g++ -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/photons/photonRecoAna.d Performance/photons/photonRecoAna.C -o Performance/photons/photonRecoAna.exe"

runCmd="time -p"
runCmdStr="time -p"
if [[ $USER == "tatar" ]]; then
  runCmd="$HOME/code/scripts/myRun.sh"
  runCmdStr="myRun"
fi

$runCmd Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.photonGun.conf /mnt/hadoop/cms/store/user/tatar/EGamma/SingleGammaFlatPt10To200_pythia8_Hydjet/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SingleGammaFlatPt10To200_pythia8_Hydjet.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SingleGammaFlatPt10To200_pythia8_Hydjet.log &
echo "$runCmdStr Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.photonGun.conf /mnt/hadoop/cms/store/user/tatar/EGamma/SingleGammaFlatPt10To200_pythia8_Hydjet/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SingleGammaFlatPt10To200_pythia8_Hydjet.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SingleGammaFlatPt10To200_pythia8_Hydjet.log &"


$runCmd Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.photonGun.GED.conf /mnt/hadoop/cms/store/user/tatar/EGamma/SingleGammaFlatPt10To200_pythia8_Hydjet/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_GED_SingleGammaFlatPt10To200_pythia8_Hydjet.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_GED_SingleGammaFlatPt10To200_pythia8_Hydjet.log &
echo "$runCmdStr Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.photonGun.GED.conf /mnt/hadoop/cms/store/user/tatar/EGamma/SingleGammaFlatPt10To200_pythia8_Hydjet/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_GED_SingleGammaFlatPt10To200_pythia8_Hydjet.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_GED_SingleGammaFlatPt10To200_pythia8_Hydjet.log &"

## SIG
$runCmd Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.photonGun.SIG.conf /mnt/hadoop/cms/store/user/tatar/EGamma/SingleGammaFlatPt10To200_pythia8_Hydjet/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_SingleGammaFlatPt10To200_pythia8_Hydjet.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_SingleGammaFlatPt10To200_pythia8_Hydjet.log &
echo "$runCmdStr Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.photonGun.SIG.conf /mnt/hadoop/cms/store/user/tatar/EGamma/SingleGammaFlatPt10To200_pythia8_Hydjet/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_SingleGammaFlatPt10To200_pythia8_Hydjet.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_SingleGammaFlatPt10To200_pythia8_Hydjet.log &"


$runCmd Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.photonGun.SIG.GED.conf /mnt/hadoop/cms/store/user/tatar/EGamma/SingleGammaFlatPt10To200_pythia8_Hydjet/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_GED_SingleGammaFlatPt10To200_pythia8_Hydjet.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_GED_SingleGammaFlatPt10To200_pythia8_Hydjet.log &
echo "$runCmdStr Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.photonGun.SIG.GED.conf /mnt/hadoop/cms/store/user/tatar/EGamma/SingleGammaFlatPt10To200_pythia8_Hydjet/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_GED_SingleGammaFlatPt10To200_pythia8_Hydjet.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_GED_SingleGammaFlatPt10To200_pythia8_Hydjet.log &"

