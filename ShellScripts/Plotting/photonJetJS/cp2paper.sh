#!/bin/bash

if [ $# -lt 1 ]; then
  echo "Usage: ./ShellScripts/Plotting/photonJetJS/cp2paper.sh [paper trunk dir]"
  echo "Example: ./ShellScripts/Plotting/photonJetJS/cp2paper.sh /path/to/myDocuments/XXX-YY-ZZZ/papers/XXX-YY-ZZZ/trunk/"
  exit 1
fi

destinationDir=$1
sourceDir="./"

sourceFiles=(
"photonjetJS_compact_fig2.pdf"
"js_60_30_gxi0.pdf"
"js_60_30_gxi0_cent030.pdf"
"js_pp_data_mc.pdf"
);

destinationFiles=(
"figures/jetshape_phoet60_jetpt30_paper.pdf"
"figures/supplementary/jetshape_phoet60_jetpt30.pdf"
"figures/supplementary/jetshape_phoet60_jetpt30_twoCentBins.pdf"
"figures/supplementary/jetshape_phoet60_jetpt30_pp_data_mc.pdf"
);

arrayIndices=${!sourceFiles[*]}
for i1 in $arrayIndices
do
    srcFile=${sourceFiles[i1]}
    destFile=${destinationFiles[i1]}
    cp $sourceDir$srcFile $destinationDir$destFile
    echo "cp $sourceDir$srcFile $destinationDir$destFile"
done

