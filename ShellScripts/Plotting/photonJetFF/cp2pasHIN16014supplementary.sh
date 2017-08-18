#!/bin/bash

if [ $# -lt 1 ]; then
  echo "Usage: ./ShellScripts/Plotting/photonJetFF/cp2pasHIN16014supplementary.sh [PAS trunk dir]"
  echo "Example: ./ShellScripts/Plotting/photonJetFF/cp2pasHIN16014supplementary.sh /path/to/myDocuments/HIN-16-014/notes/HIN-16-014/trunk/"
  exit 1
fi

destinationDir=$1
sourceDir="./"

sourceFiles=(
"ff_60_30_gxi0_cent030.pdf"
"ff_60_30_gxi1_cent030.pdf"
"plotTechnicalTrk.pdf"
"plotTechnicalJet.pdf"
"plotTechnicalSumIso.pdf"
);

destinationFiles=(
"figures/supplementary/xijet_phoet60_jetpt30_twoCentBins.pdf"
"figures/supplementary/xigamma_phoet60_jetpt30_twoCentBins.pdf"
"figures/supplementary/xijet_bkgTrk_subtraction.pdf"
"figures/supplementary/xijet_bkgJet_subtraction.pdf"
"figures/supplementary/sumIso_Pythia_Hydjet_sigbkg.pdf"
);

arrayIndices=${!sourceFiles[*]}
for i1 in $arrayIndices
do
    srcFile=${sourceFiles[i1]}
    destFile=${destinationFiles[i1]}
    cp $sourceDir$srcFile $destinationDir$destFile
    echo "cp $sourceDir$srcFile $destinationDir$destFile"
done

