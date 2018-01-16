#!/bin/bash

if [ $# -lt 1 ]; then
  echo "Usage: ./ShellScripts/Plotting/photonJetFF/cp2pasHIN16014supplementary.sh [PAPER trunk dir]"
  echo "Example: ./ShellScripts/Plotting/photonJetFF/cp2pasHIN16014supplementary.sh /path/to/myDocuments/HIN-16-014/papers/HIN-16-014/trunk/"
  exit 1
fi

destinationDir=$1
sourceDir="./"

sourceFiles=(
"ff_60_30_gxi0.pdf"
"ff_60_30_gxi1.pdf"
"ff_60_30_gxi0_cent030.pdf"
"ff_60_30_gxi1_cent030.pdf"
"plotTechnicalTrk.pdf"
"plotTechnicalJet.pdf"
"plotTechnicalSumIso.pdf"
### xi phase space
"Plotting/photonJetFF/jetpt_vs_xi.pdf"
"Plotting/photonJetFF/trackpt_vs_xi.pdf"
"Plotting/photonJetFF/jetpt_vs_xi_plusminus_trkPt1_deta0p3.pdf"
"Plotting/photonJetFF/trackpt_vs_xi_plusminus_jetPt30_deta0p3.pdf"
"Plotting/photonJetFF/phopt_vs_xi.pdf"
"Plotting/photonJetFF/trackpt_vs_xigamma.pdf"
"Plotting/photonJetFF/phopt_vs_xi_trkPt1.pdf"
"Plotting/photonJetFF/trackpt_vs_xigamma_phoPt60.pdf"
);

destinationFiles=(
"figures/supplementary/xijet_phoet60_jetpt30.pdf"
"figures/supplementary/xigamma_phoet60_jetpt30.pdf"
"figures/supplementary/xijet_phoet60_jetpt30_twoCentBins.pdf"
"figures/supplementary/xigamma_phoet60_jetpt30_twoCentBins.pdf"
"figures/supplementary/xijet_bkgTrk_subtraction.pdf"
"figures/supplementary/xijet_bkgJet_subtraction.pdf"
"figures/supplementary/sumIso_Pythia_Hydjet_sigbkg.pdf"
### xi phase space
"figures/supplementary/fnc_jetpt_vs_xijet.pdf"
"figures/supplementary/fnc_trackpt_vs_xijet.pdf"
"figures/supplementary/fnc_jetpt_vs_xijet_trkPt1_deta0p3.pdf"
"figures/supplementary/fnc_trackpt_vs_xijet_jetPt30_deta0p3.pdf"
"figures/supplementary/fnc_phopt_vs_xigamma.pdf"
"figures/supplementary/fnc_trackpt_vs_xigamma.pdf"
"figures/supplementary/fnc_phopt_vs_xigamma_trkPt1.pdf"
"figures/supplementary/fnc_trackpt_vs_xigamma_phoPt60.pdf"
);

arrayIndices=${!sourceFiles[*]}
for i1 in $arrayIndices
do
    srcFile=${sourceFiles[i1]}
    destFile=${destinationFiles[i1]}
    cp $sourceDir$srcFile $destinationDir$destFile
    echo "cp $sourceDir$srcFile $destinationDir$destFile"
done

