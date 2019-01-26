#!/bin/bash

# must be run after runPrintHEPDataValues.sh

tables=($(seq 1 1 9))

outputBase="./Data/photonJetJS/hepdata"

oldLine1="  values:"
oldLine2="  values:"

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
arrayIndices=${!tables[*]}
for i1 in $arrayIndices
do
    table=${tables[i1]}

    outputFile=$outputBase"/data"$table".yaml"

    newLine1="- header: {name: R}"

    newLine2=""
    if [ $i1 -lt 1 ]; then
      newLine2="- header: {name: RHO_(R) PP}"
    elif [ $i1 -lt 5 ]; then
      newLine2="- header: {name: RHO_(R) PBPB}"
    else
      newLine2="- header: {name: PBPB / PP}"
    fi

    lineCentrality=""
    if [ $(($i1 % 4)) -eq 0 ]; then
      lineCentrality="  - {name: CENTRALITY, value: '50 - 100%'}"
    elif [ $(($i1 % 4)) -eq 1 ]; then
      lineCentrality="  - {name: CENTRALITY, value: '30 - 50%'}"
    elif [ $(($i1 % 4)) -eq 2 ]; then
      lineCentrality="  - {name: CENTRALITY, value: '10 - 30%'}"
    else
      lineCentrality="  - {name: CENTRALITY, value: '0 - 10%'}"
    fi

    # add header for "independent variables"
    oldLine=$oldLine1
    newLine="independent_variables:\n"${newLine1}"\n"$oldLine
    sed -i "0,/${oldLine}/ s/${oldLine}/${newLine}/" $outputFile # replace the 1st occurance in the file

    lineReaction="  - {name: RE, value: P P --> GAMMA JET PB PB --> GAMMA JET}"
    # add header following 1st "dependent variables"
    oldLine=$oldLine2
    newLine="dependent_variables:\n"${newLine2}"\n  qualifiers:\n"${lineReaction}"\n"${lineCentrality}"\n"$textKinematics"\n"$oldLine
    sed -i "0,/${oldLine}/! {0,/${oldLine}/ s;${oldLine};${newLine};}" $outputFile  # replace only the 2nd occurance in the file

    echo "modified : "$outputFile
done

