#!/bin/bash

outputFile="./Data/photonJetJS/hepdata/submission.yaml"

tables=($(seq 1 1 9))

rm -f $outputFile

echo "additional_resources: []" >> $outputFile
echo "comment: ''" >> $outputFile

arrayIndices=${!tables[*]}
for i1 in $arrayIndices
do
    table=${tables[i1]}

    figure="1"

    topBottom="top"
    if [ $table -ge 6 ]; then
      # tables 6,7,8,9
      topBottom="bottom"
    fi

    centrality=""
    if [ $table -eq 2 ] || [ $table -eq 6 ]; then
      centrality="50-100%"
    elif [ $table -eq 3 ] || [ $table -eq 7 ]; then
      centrality="30-50%"
    elif [ $table -eq 4 ] || [ $table -eq 8 ]; then
      centrality="10-30%"
    elif [ $table -eq 5 ] || [ $table -eq 9 ]; then
      centrality="0-10%"
    else
      centrality="NOCENT"
    fi

    observable=""
    obsDesc=""
    if [ $table -lt 6 ]; then
      observable="RHO_(R)"
      obsDesc="$\rho(r)$"
    else
      observable="PBPB / PP"
      obsDesc="PbPb / pp ratio of $\rho(r)$"
    fi

    description="The differential jet shape, ${obsDesc}, for jets associated with an isolated photon in ${centrality} centrality PbPb collisions."
    if [ $table -eq 1 ]; then
      description="The differential jet shape, ${obsDesc}, for jets associated with an isolated photon in pp collisions."
    elif [ $table -ge 6 ]; then
      description="${obsDesc} for ${centrality} centrality PbPb collisions."
    fi

    reaction="P P --> GAMMA JET, PB PB --> GAMMA JET"

    echo "---" >> $outputFile
    echo "name: \"Table ${table}\"" >> $outputFile
    echo "location: Data from ${topBottom} panel of Fig. ${figure}" >> $outputFile
    echo "description: ${description}" >> $outputFile
    echo "keywords:" >> $outputFile
    echo "  - {name: reactions, values: [${reaction}]}" >> $outputFile
    echo "  - {name: observables, values: [${observable}]}" >> $outputFile
    echo "  - {name: cmenergies, values: [5020.0]}" >> $outputFile
    echo "  - {name: phrases, values: []}" >> $outputFile
    echo "data_file: data${table}.yaml" >> $outputFile
    echo "data_license: {description: null, name: null, url: null}" >> $outputFile
    echo "additional_resources: []" >> $outputFile

    echo "wrote Table "$table
done

