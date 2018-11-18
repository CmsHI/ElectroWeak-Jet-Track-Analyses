#!/bin/bash

outputFile="./Data/photonJetFF/hepdata/submission.yaml"

tables=(
"1"
"2"
"3"
"4"
"5"
"6"
"7"
"8"
"9"
"10"
"11"
"12"
"13"
"14"
"15"
"16"
);

figures=(
"1"
"1"
"1"
"1"
"1"
"1"
"1"
"1"
"2"
"2"
"2"
"2"
"2"
"2"
"2"
"2"
);

topBottoms=(
"top"
"top"
"top"
"top"
"bottom"
"bottom"
"bottom"
"bottom"
"top"
"top"
"top"
"top"
"bottom"
"bottom"
"bottom"
"bottom"
);

centralities=(
"50-100%"
"30-50%"
"10-30%"
"0-10%"
"50-100%"
"30-50%"
"10-30%"
"0-10%"
"50-100%"
"30-50%"
"10-30%"
"0-10%"
"50-100%"
"30-50%"
"10-30%"
"0-10%"
);

#descriptions=(
#"XI_(JET) distribution for jets associated with an isolated photon in 50-100% centrality PbPb collisions."
#"XI_(JET) distribution for jets associated with an isolated photon in pp collisions. The resolutions of the measured jet energy and azimuthal angle are smeared to match those of the 50-100% centrality PbPb sample."
#"XI_(JET) distribution for jets associated with an isolated photon in 30-50% centrality PbPb collisions."
#"XI_(JET) distribution for jets associated with an isolated photon in pp collisions. The resolutions of the measured jet energy and azimuthal angle are smeared to match those of the 30-50% centrality PbPb sample."
#"XI_T_(GAMMA) distribution for jets associated with an isolated photon in 50-100% centrality PbPb collisions."
#"XI_T_(GAMMA) distribution for jets associated with an isolated photon in pp collisions. The resolutions of the measured jet energy and azimuthal angle are smeared to match those of the 50-100% centrality PbPb sample."
#"XI_T_(GAMMA) distribution for jets associated with an isolated photon in 30-50% centrality PbPb collisions."
#"XI_T_(GAMMA) distribution for jets associated with an isolated photon in pp collisions. The resolutions of the measured jet energy and azimuthal angle are smeared to match those of the 30-50% centrality PbPb sample."
#);

observables=(
"1/N(JET) * DN(TRK))/DXI_(JET)"
"1/N(JET) * DN(TRK))/DXI_(JET)"
"1/N(JET) * DN(TRK))/DXI_(JET)"
"1/N(JET) * DN(TRK))/DXI_(JET)"
"PBPB / PP"
"PBPB / PP"
"PBPB / PP"
"PBPB / PP"
"1/N(JET) * DN(TRK))/DXI_T_(GAMMA)"
"1/N(JET) * DN(TRK))/DXI_T_(GAMMA)"
"1/N(JET) * DN(TRK))/DXI_T_(GAMMA)"
"1/N(JET) * DN(TRK))/DXI_T_(GAMMA)"
"PBPB / PP"
"PBPB / PP"
"PBPB / PP"
"PBPB / PP"
);

obsDescList=(
"XI_(JET)"
"XI_(JET)"
"XI_(JET)"
"XI_(JET)"
"PbPb / pp ratio of XI_(JET)"
"PbPb / pp ratio of XI_(JET)"
"PbPb / pp ratio of XI_(JET)"
"PbPb / pp ratio of XI_(JET)"
"XI_T_(GAMMA)"
"XI_T_(GAMMA)"
"XI_T_(GAMMA)"
"XI_T_(GAMMA)"
"PbPb / pp ratio of XI_T_(GAMMA)"
"PbPb / pp ratio of XI_T_(GAMMA)"
"PbPb / pp ratio of XI_T_(GAMMA)"
"PbPb / pp ratio of XI_T_(GAMMA)"
);

reactions=(
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
"P P --> X, PB PB --> X"
);

rm -f $outputFile

echo "additional_resources: []" >> $outputFile
echo "comment: ''" >> $outputFile

arrayIndices=${!tables[*]}
for i1 in $arrayIndices
do
    table=${tables[i1]}

    centrality=${centralities[i1]}

    obsDesc=${obsDescList[i1]}
    description="${obsDesc} distributions for jets associated with an isolated photon in pp and ${centrality} centrality PbPb collisions. The resolutions of the measured jet energy and azimuthal angle in pp are smeared to match those in the PbPb sample."
    if [[ $obsDesc = *ratio* ]]; then
      description="${obsDesc} distributions for ${centrality} centrality collisions. The resolutions of the measured jet energy and azimuthal angle in pp are smeared to match those in the PbPb sample."
    fi

    reaction=${reactions[i1]}

    echo "---" >> $outputFile
    echo "name: \"Table ${table}\"" >> $outputFile
    echo "location: Data from ${topBottoms[i1]} panel of Fig. ${figures[i1]}" >> $outputFile
    echo "description: ${description}" >> $outputFile
    echo "keywords:" >> $outputFile
    echo "  - {name: reactions, values: [${reaction}]}" >> $outputFile
    echo "  - {name: observables, values: [${observables[i1]}]}" >> $outputFile
    echo "  - {name: cmenergies, values: [5020.0]}" >> $outputFile
    echo "  - {name: phrases, values: []}" >> $outputFile
    echo "data_file: data${table}.yaml" >> $outputFile
    echo "data_license: {description: null, name: null, url: null}" >> $outputFile
    echo "additional_resources: []" >> $outputFile

    echo "wrote Table "$table
done

