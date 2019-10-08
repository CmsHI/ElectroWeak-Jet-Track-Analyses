#!/bin/bash

#myLine=$(grep -A 8 --color "signal efficiency = 0.9" ${EWJTAOUT}"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaReadXML_tmvaTrainID_sumIso4_CutsGA.log | grep --color -A 1 "phoSigmaIEtaIEta_2012" | cut -d ":" -f 3)
#myLine=$(grep -A 8 --color "signal efficiency = 0.9" ${EWJTAOUT}"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaReadXML_tmvaTrainID_sumIso4_CutsGA.log | grep --color -A 1 "phoHoverE" | cut -d ":" -f 3)

#folderName="pp17"
folderName="pbpb18"
EBEE="EB"
#EBEE="EE"
#EBEE="EE1"

WPEFF=80
substrInputWP=""
substrConfigVar=""
substrGrep=""

if [ $WPEFF -eq 80 ]; then
  substrInputWP=""
  substrConfigVar="80EFF_"${EBEE}
  substrGrep="0.9"
elif [ $WPEFF -eq 70 ]; then
  substrInputWP="_WP80"
  substrConfigVar="70EFF_"${EBEE}
  substrGrep="0.8"
elif [ $WPEFF -eq 60 ]; then
  substrInputWP="_WP70"
  substrConfigVar="60EFF_"${EBEE}
  substrGrep="0.7"
else
  echo "invalid WP efficiency : "${WPEFF}
  exit
fi


inputList=(
### R3
## phoHoverE 
$EWJTAOUT"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaReadXML_tmvaTrainID"${substrInputWP}"_sumIso3_CutsGA.log"
$EWJTAOUT"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaReadXML_tmvaTrainID"${substrInputWP}"_pfSumIso3_subUE_CutsGA.log"
$EWJTAOUT"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaReadXML_tmvaTrainID"${substrInputWP}"_pfSumIso3_subUE_cIsopT2_CutsGA.log"
## phoSigmaIEtaIEta_2012
$EWJTAOUT"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaReadXML_tmvaTrainID"${substrInputWP}"_sumIso3_CutsGA.log"
$EWJTAOUT"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaReadXML_tmvaTrainID"${substrInputWP}"_pfSumIso3_subUE_CutsGA.log"
$EWJTAOUT"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaReadXML_tmvaTrainID"${substrInputWP}"_pfSumIso3_subUE_cIsopT2_CutsGA.log"
## sumIso
$EWJTAOUT"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaReadXML_tmvaTrainID"${substrInputWP}"_sumIso3_CutsGA.log"
## pfSumIso
$EWJTAOUT"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaReadXML_tmvaTrainID"${substrInputWP}"_pfSumIso3_subUE_CutsGA.log"
$EWJTAOUT"/Performance/photons/identification/"${folderName}"/"${EBEE}"/tmvaReadXML_tmvaTrainID"${substrInputWP}"_pfSumIso3_subUE_cIsopT2_CutsGA.log"
);

variableList=(
### R3
"phoHoverE"
"phoHoverE"
"phoHoverE"
##
"phoSigmaIEtaIEta_2012"
"phoSigmaIEtaIEta_2012"
"phoSigmaIEtaIEta_2012"
#"phoSigmaIEtaIEta_2012" # rest of the line not important
##
"pho_ecalClusterIsoR3+pho_hcalRechitIsoR3+pho_trackIsoR3PtCut20"
##
"pfcIso3subUE+pfpIso3subUE+pfnIso3subUE"
"pfcIso3pTgt2p0subUE+pfpIso3subUE+pfnIso3subUE"
);

configVarList=(
### R3
"HOE_MAX_"${substrConfigVar}"_SUMISO3"
"HOE_MAX_"${substrConfigVar}"_PFSUMISO3SUBUE"
"HOE_MAX_"${substrConfigVar}"_PFSUMISO3SUBUE_CPT2"
##
"SIEIE_MAX_"${substrConfigVar}"_SUMISO3"
"SIEIE_MAX_"${substrConfigVar}"_PFSUMISO3SUBUE"
"SIEIE_MAX_"${substrConfigVar}"_PFSUMISO3SUBUE_CPT2"
##
"SUMISO3_MAX_"${substrConfigVar}""
##
"PFSUMISO3SUBUE_MAX_"${substrConfigVar}""
"PFSUMISO3SUBUE_CPT2_MAX_"${substrConfigVar}""
);

configFile="Configurations/Performance/photons/identification/"${folderName}"/tmvaTrainID.frag.conf"

arrayIndices=${!configVarList[*]}
for i1 in $arrayIndices
do
    inputFile=${inputList[i1]}
    echo "reading file : "${inputFile}
    variable=${variableList[i1]}
    configVar=${configVarList[i1]}
    varMaxValue=$(grep -A 25 --color "signal efficiency = ${substrGrep}" ${inputFile} | grep --color -A 1 ${variable} | cut -d ":" -f 3)
    
    # xargs trims whitespace automatically : https://stackoverflow.com/a/12973694
    varMaxValue=$(echo ${varMaxValue} | xargs)

    echo "varMaxValue = "$varMaxValue" for ${configVar}"

    subStrSearch=${configVar}
    newLine=${configVar}"\ =\ "${varMaxValue}
    #echo $newLine

    sed -i "s/${subStrSearch} .*/${newLine}/" ${configFile}
done

echo "replaced values in file : "${configFile}

