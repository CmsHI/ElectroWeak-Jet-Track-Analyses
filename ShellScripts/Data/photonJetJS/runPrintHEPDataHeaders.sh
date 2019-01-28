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
    if [ $table -eq 1 ]; then
      newLine2="- header: {name: RHO_(R) PP}"
    elif [ $table -le 5 ]; then
      newLine2="- header: {name: RHO_(R) PBPB}"
    else
      newLine2="- header: {name: PBPB / PP}"
    fi

    lineCentrality=""
    if [ $table -eq 2 ] || [ $table -eq 6 ]; then
      lineCentrality="  - {name: CENTRALITY, value: '50 - 100%'}"
    elif [ $table -eq 3 ] || [ $table -eq 7 ]; then
      lineCentrality="  - {name: CENTRALITY, value: '30 - 50%'}"
    elif [ $table -eq 4 ] || [ $table -eq 8 ]; then
      lineCentrality="  - {name: CENTRALITY, value: '10 - 30%'}"
    elif [ $table -eq 5 ] || [ $table -eq 9 ]; then
      lineCentrality="  - {name: CENTRALITY, value: '0 - 10%'}"
    fi

    lineReaction="  - {name: RE, value: P P --> GAMMA JET}"
    if [ $table -ge 2 ] && [ $table -le 5 ]; then
      lineReaction="  - {name: RE, value: PB PB --> GAMMA JET}"
    elif [ $table -ge 6 ] && [ $table -le 9 ]; then
      lineReaction="  - {name: RE, value: P P --> GAMMA JET PB PB --> GAMMA JET}"
    fi

    # add header for "independent variables"
    oldLine=$oldLine1
    newLine="independent_variables:\n"${newLine1}"\n"$oldLine
    sed -i "0,/${oldLine}/ s/${oldLine}/${newLine}/" $outputFile # replace the 1st occurance in the file

    # add header following 1st "dependent variables"
    oldLine=$oldLine2
    if [ -z "$lineCentrality" ]; then
      newLine="dependent_variables:\n"${newLine2}"\n  qualifiers:\n"${lineReaction}"\n"$textKinematics"\n"$oldLine
    else
      newLine="dependent_variables:\n"${newLine2}"\n  qualifiers:\n"${lineReaction}"\n"${lineCentrality}"\n"$textKinematics"\n"$oldLine
    fi
    sed -i "0,/${oldLine}/! {0,/${oldLine}/ s;${oldLine};${newLine};}" $outputFile  # replace only the 2nd occurance in the file

    echo "modified : "$outputFile
done

