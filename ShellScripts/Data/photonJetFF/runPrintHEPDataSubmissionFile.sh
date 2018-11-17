#!/bin/bash

outputFile="./Data/photonJetFF/hepdata/submission.yaml"

tables=(
"1"
"2"
"3"
"4"
);

figures=(
"1"
"1"
"2"
"2"
);

topBottoms=(
"top"
"top"
"top"
"top"
#"bottom"
#"bottom"
);

isXiJet=(
1
1
0
0
);

centralities=(
"50-100%"
"50-100%"
"50-100%"
"50-100%"
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

reactions=(
"PB PB"
"P P"
"PB PB"
"P P"
);

observables=(
"1/N(JET) * DN(TRK))/DXI_(JET)"
"1/N(JET) * DN(TRK))/DXI_(JET)"
"1/N(JET) * DN(TRK))/DXI_T_(PHOTON)"
"1/N(JET) * DN(TRK))/DXI_T_(PHOTON)"
);

rm -f $outputFile

echo "additional_resources: []" >> $outputFile
echo "comment: ''" >> $outputFile

arrayIndices=${!tables[*]}
for i1 in $arrayIndices
do
    table=${tables[i1]}

    xiStr="XI_(JET)"
    if [ ${isXiJet[i1]} == 0 ]; then
      xiStr="XI_T_(GAMMA)"
    fi
    centrality=${centralities[i1]}

    description="${xiStr} distribution for jets associated with an isolated photon in ${centrality} centrality PbPb collisions."
    reaction=${reactions[i1]}
    if [[ $reaction != "PB PB" ]]; then
      description="${xiStr} distribution for jets associated with an isolated photon in pp collisions. The resolutions of the measured jet energy and azimuthal angle are smeared to match those of the ${centrality} centrality PbPb sample."
    fi

    echo "---" >> $outputFile
    echo "name: \"Table ${table}\"" >> $outputFile
    echo "location: Data from ${topBottoms[i1]} panel of Fig. ${figures[i1]}" >> $outputFile
    echo "description: ${description}" >> $outputFile
    echo "keywords:" >> $outputFile
    echo "  - {name: reactions, values: [${reaction} --> X]}" >> $outputFile
    echo "  - {name: observables, values: [${observables[i1]}]}" >> $outputFile
    echo "  - {name: cmenergies, values: [5020.0]}" >> $outputFile
    echo "  - {name: phrases, values: []}" >> $outputFile
    echo "data_file: data${table}.yaml" >> $outputFile
    echo "data_license: {description: null, name: null, url: null}" >> $outputFile
    echo "additional_resources: []" >> $outputFile

    echo "wrote Table "$table
done

