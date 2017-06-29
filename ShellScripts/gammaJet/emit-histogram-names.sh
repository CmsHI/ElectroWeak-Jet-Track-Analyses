#!/bin/bash

if [[ $# -lt 2 ]]; then
    echo "Usage: ./ShellScripts/gammaJet/emit-histogram-names.sh [output] [flags] {suffix}"
    echo "[flags] 0x1111 for all, 0x0101 for data only, 0x0011 for pbpb only"
    exit 1;
fi

[ -f $1 ] && rm $1

[[ $# -eq 3 ]] && SUFFIX=$3 || SUFFIX=""

DATATYPES=(PbPb_Data PbPb_MC pp_Data pp_MC)
TYPES=()
for b in 3 2 1 0
do
    (($2 & $((2 ** b)))) && TYPES+=("${DATATYPES[b]}")
done

OBS=(xjg dphi dphi ptJet ptJet)
OBS_SUFFIX=("" "" "_rebin" "" "_rebin");

for h in ${!TYPES[@]}; do
    for i in ${!OBS[@]}; do
        for j in $(seq 0 7); do
            for k in $(seq 0 6); do
                echo h1D_${OBS[i]}_ptBin${j}_hiBin${k}_${TYPES[h]}${OBS_SUFFIX[i]}${SUFFIX} >> $1
            done
        done
    done
done

DERIVED_OBS=(xjg_mean rjg dphi_width dphi_pedestal)

for h in ${!TYPES[@]}; do
    for i in ${DERIVED_OBS[@]}; do
        for j in $(seq 0 7); do
            echo h1D_${DERIVED_OBS[i]}_centBinAll_ptBin${j}_${TYPES[h]}${SUFFIX} >> $1
        done

        for j in $(seq 0 6); do
            echo h1D_${DERIVED_OBS[i]}_ptBinAll_hiBin${j}_${TYPES[h]}${SUFFIX} >> $1
        done
    done
done

for j in $(seq 0 7); do
    for k in $(seq 0 6); do
        echo h1D_iaa_ptBin${j}_hiBin${k}_rebin${SUFFIX} >> $1
    done
done
