#!/bin/bash

outputFile="./Data/photonJetFF/hepdata/submission.yaml"

tables=($(seq 1 1 16))

rm -f $outputFile

echo "additional_resources: []" >> $outputFile
echo "comment: ''" >> $outputFile

arrayIndices=${!tables[*]}
for i1 in $arrayIndices
do
    table=${tables[i1]}

    figure="1"
    if [ $table -gt "8" ]; then
       figure="2"
    fi

    topBottom="top"
    if [ $(((table-1) % 8)) -ge 4 ]; then
      # tables 5,6,7,8,13,14,15,16
      topBottom="bottom"
    fi

    centrality=""
    if [ $(((table-1) % 4)) -eq 0 ]; then
      centrality="50-100%"
    elif [ $(((table-1) % 4)) -eq 1 ]; then
      centrality="30-50%"
    elif [ $(((table-1) % 4)) -eq 2 ]; then
      centrality="10-30%"
    else
      centrality="0-10%"
    fi

    observable=""
    obsDesc=""
    if [ $(((table-1) / 4)) -eq 0 ]; then
      observable="1/N(JET) * DN(TRK))/DXI_(JET)"
      obsDesc="$\xi^\mathrm{jet}$"
    elif [ $(((table-1) / 4)) -eq 1 ]; then
      observable="PBPB / PP"
      obsDesc="PbPb / pp ratio of $\xi^\mathrm{jet}$"
    elif [ $(((table-1) / 4)) -eq 2 ]; then
      observable="1/N(JET) * DN(TRK))/DXI_T_(GAMMA)"
      obsDesc="$\xi^\gamma_{\mathrm{T}}$"
    else
      observable="PBPB / PP"
      obsDesc="PbPb / pp ratio of $\xi^\gamma_{\mathrm{T}}$"
    fi

    description="${obsDesc} distributions for jets associated with an isolated photon in pp and ${centrality} centrality PbPb collisions. The resolutions of the measured jet energy and azimuthal angle in pp are smeared to match those in the PbPb sample."
    if [[ $obsDesc = *ratio* ]]; then
      description="${obsDesc} distributions for ${centrality} centrality collisions. The resolutions of the measured jet energy and azimuthal angle in pp are smeared to match those in the PbPb sample."
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

