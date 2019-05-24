#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/tmvaReadXML.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
$EWJTAOUT"/Performance/photons/identification/pbpb18/tmvaTrainID_EB_minPt40_HoE_sieie_CutsGA.xml"
);

methodNameList=(
"CutsGA"
);

variableList=(
"phoHoverE,phoSigmaIEtaIEta_2012"
);

sigEffList=(
0.6,0.65,0.7,0.75,0.8
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
$outDirBase"/Performance/photons/identification/pbpb18/tmvaReadXML_tmvaTrainID_EB_minPt40_HoE_sieie_CutsGA.root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    inputFile=${inputList[i1]}
    methodName=${methodNameList[i1]}
    variables=${variableList[i1]}
    sigEff=${sigEffList[i1]}
    spectators=${spectatorList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFileLOG}")
    mkdir -p $outDir
    $runCmd $progPath $inputFile $methodName $variables $outputFile $sigEff --spectators=${spectators} &> $outputFileLOG &
    echo "$runCmd $progPath $inputFile $methodName $variables $outputFile $sigEff --spectators=${spectators} &> $outputFileLOG &"
done

