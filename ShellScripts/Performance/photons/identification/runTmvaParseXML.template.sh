#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/tmvaParseXML.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
$EWJTAOUT"/Performance/photons/identification/pbpb18/dataset/weights/tmvatrainID_EB_minPt40_HoE_sieie_CutsGA.weights.xml"
);

methodNameList=(
"CutsGA"
);

variableList=(
"phoHoverE,phoSigmaIEtaIEta_2012"
);

sigEffList=(
0.7
);

specList1="phoEt,phoEta,phoPhi,phoSCEta,phoSCPhi,phoHoverE,pho_ecalClusterIsoR4,pho_hcalRechitIsoR4,pho_trackIsoR4PtCut20,pho_genMatchedIndex"
spectatorList=(
${specList1}
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
$outDirBase"/Performance/photons/identification/pbpb18/tmvaParseXML_tmvaTrainID_EB_minPt40_HoE_sieie.log"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    inputFile=${inputList[i1]}
    methodName=${methodNameList[i1]}
    variables=${variableList[i1]}
    sigEff=${sigEffList[i1]}
    spectators=${spectatorList[i1]}
    outputFileLOG=${outList[i1]}
    outDir=$(dirname "${outputFileLOG}")
    mkdir -p $outDir
    $runCmd $progPath $inputFile $methodName $variables $sigEff --spectators=${spectators} &> $outputFileLOG &
    echo "$runCmd $progPath $inputFile $methodName $variables $sigEff --spectators=${spectators} &> $outputFileLOG &"
done

