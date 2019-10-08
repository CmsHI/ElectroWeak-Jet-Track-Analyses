#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/tmvaReadXML.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

#folderName="pp17"
folderName="pbpb18"
EBEE="EB"
#EBEE="EE"
#EBEE="EE1"

prefixWP=""
sigEffList1=0.5,0.6,0.7,0.8,0.9

#prefixWP="_WP80"
#sigEffList1=0.5,0.6,0.7,0.8

#prefixWP="_WP70"
#sigEffList1=0.5,0.6,0.7

#prefixWP="_WP60"
#sigEffList1=0.5,0.6
inputList=(
## R3
$EWJTAOUT"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaTrainID"${prefixWP}"_sumIso3_CutsGA.xml"
$EWJTAOUT"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaTrainID"${prefixWP}"_pfSumIso3_subUE_CutsGA.xml"
$EWJTAOUT"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaTrainID"${prefixWP}"_pfSumIso3_subUE_cIsopT2_CutsGA.xml"
$EWJTAOUT"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaTrainID"${prefixWP}"_pfSumIso3_subUEec_cIsopT2_CutsGA.xml"
);

methodNameList1="CutsGA"
methodNameList=(
${methodNameList1}
);

variableList=(
## R3
"phoHoverE;phoSigmaIEtaIEta_2012;pho_ecalClusterIsoR3+pho_hcalRechitIsoR3+pho_trackIsoR3PtCut20"
"phoHoverE;phoSigmaIEtaIEta_2012;pfcIso3subUE+pfpIso3subUE+pfnIso3subUE"
"phoHoverE;phoSigmaIEtaIEta_2012;pfcIso3pTgt2p0subUE+pfpIso3subUE+pfnIso3subUE"
"phoHoverE;phoSigmaIEtaIEta_2012;pfcIso3pTgt2p0subUEec+pfpIso3subUEec+pfnIso3subUEec"
);

#specList1="phoEt,phoEta,phoPhi,phoSCEta,phoSCPhi,phoHoverE,pho_ecalClusterIsoR4,pho_hcalRechitIsoR4,pho_trackIsoR4PtCut20,pho_genMatchedIndex"
specList1="phoEt,phoEta,phoPhi,phoSCEta,phoSCPhi,phoHoverE,pho_ecalClusterIsoR4,pho_hcalRechitIsoR4,pho_trackIsoR4PtCut20,pho_genMatchedIndex,hiBin,rho"
spectatorList=(
${specList1}
);

sigEffList=(
${sigEffList1}
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
#$outDirBase"/Performance/photons/identification/pbpb18/tmvaReadXML_tmvaTrainID_EB_minPt40_HoE_CutsGA.root"
);

arrayIndices=${!inputList[*]}
for i1 in $arrayIndices
do
    inputFile=${inputList[i1]}
    #methodName=${methodNameList[i1]}
    methodName=${methodNameList1}
    variables=${variableList[i1]}
    #spectators=${spectatorList[i1]}
    spectators=${specList1}
    #sigEff=${sigEffList[i1]}
    sigEff=${sigEffList1}
    #outputFile=${outList[i1]}
    outputFile="${inputFile/tmvaTrainID_/tmvaReadXML_tmvaTrainID_}"
    outputFile="${outputFile/.xml/.root}"
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFileLOG}")
    mkdir -p $outDir
    $runCmd $progPath $inputFile $methodName $variables $outputFile --spectators=${spectators} --signalEffs=${sigEff} &> $outputFileLOG &
    echo "$runCmd $progPath $inputFile $methodName $variables $outputFile --spectators=${spectators} --signalEffs=${sigEff} &> $outputFileLOG &"
    wait
done

