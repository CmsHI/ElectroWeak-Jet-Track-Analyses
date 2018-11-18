#!/bin/bash

# must be run after runPrintHEPData.sh

outputBase="./Data/photonJetFF/hepdata"
outputFiles=(
$outputBase"/data1.yaml"
$outputBase"/data2.yaml"
);

oldLine1="  values:"
newLines1=(
"- header: {name: XI_(JET)}"
"- header: {name: XI_T_(GAMMA)}"
);

oldLine2="  values:"
newLines2=(
"- header: {name: 1/N(JET) * DN(TRK))/DXI_(JET) PP}"
"- header: {name: 1/N(JET) * DN(TRK))/XI_T_(GAMMA) PP}"
);

newLines3=(
"- header: {name: 1/N(JET) * DN(TRK))/DXI_(JET) PBPB}"
"- header: {name: 1/N(JET) * DN(TRK))/XI_T_(GAMMA) PBPB}"
);

linesReaction=(
"  - {name: RE, value: P P --> X PB PB --> X}"
"  - {name: RE, value: P P --> X PB PB --> X}"
);

linesCentrality=(
"  - {name: CENTRALITY, value: '50 - 100%'}"
"  - {name: CENTRALITY, value: '50 - 100%'}"
);

linesKinematics=(
"  - {name: PT(GAMMA), units: GEV/C, value: '> 60'}"
"  - {name: '|ETA(GAMMA)|', value: '< 1.44'}"
"  - {name: DELTA(PHI_JGAMMA), value: '> 7 * PI / 8'}"
"  - {name: JET ALGO, value: 'ANTI-K_T JET R = 0.3'}"
"  - {name: PT(JET), units: GEV/C, value: '> 30'}"
"  - {name: '|ETA(JET)|', value: '< 1.6'}"
"  - {name: PT(TRK), units: GEV/C, value: '> 1'}"
);

textKinematics=""

for i1 in ${!linesKinematics[*]}
do
  if [ $i1 == 0 ]; then
    textKinematics=${linesKinematics[i1]}
  else
    textKinematics=$textKinematics"\n"${linesKinematics[i1]}
  fi
done

#set -x
arrayIndices=${!outputFiles[*]}
for i1 in $arrayIndices
do
    outputFile=${outputFiles[i1]}

    # add header for "independent variables"
    oldLine=$oldLine1
    newLine="independent_variables:\n"${newLines1[i1]}"\n"$oldLine
    sed -i "0,/${oldLine}/ s/${oldLine}/${newLine}/" $outputFile # replace the 1st occurance in the file

    # add header following 1st "dependent variables"
    oldLine=$oldLine2
    newLine="dependent_variables:\n"${newLines2[i1]}"\n  qualifiers:\n"${linesReaction[i1]}"\n"${linesCentrality[i1]}"\n"$textKinematics"\n"$oldLine
    sed -i "0,/${oldLine}/! {0,/${oldLine}/ s;${oldLine};${newLine};}" $outputFile  # replace only the 2nd occurance in the file

    # add header following 2nd "dependent variables"
    oldLine=$oldLine2
    newLine=${newLines3[i1]}"\n  qualifiers:\n"${linesReaction[i1]}"\n"${linesCentrality[i1]}"\n"$textKinematics"\n"$oldLine
    outputFileTmp=$outputFile".tmp"
    mv $outputFile $outputFileTmp
    # replace only the 3rd occurance in the file
    awk -v oldLineAWK="$oldLine" -v newLineAWK="$newLine" '/values/{count++;if(count==3){sub(oldLineAWK, newLineAWK)}}1' $outputFileTmp > $outputFile
    rm $outputFileTmp

    echo "modified : "$outputFile
done

