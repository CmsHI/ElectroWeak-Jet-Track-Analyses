#!/bin/bash

[ -f $1 ] && rm $1

DATATYPE=(PbPb_Data PbPb_MC pp_Data pp_MC)

BASE_OBS=(xjg dphi dphi ptJet ptJet)
SUFFIX=("" "" "_rebin" "" "_rebin");

for h in $(seq 0 3); do
    for i in $(seq 0 4); do
        for j in $(seq 0 7); do
            for k in $(seq 0 6); do
                echo h1D_${BASE_OBS[i]}_ptBin${j}_hiBin${k}_${DATATYPE[h]}${SUFFIX[i]} >> $1
            done
        done
    done
done

DERIVED_OBS=(xjg_mean rjg dphi_width dphi_pedestal)

for h in $(seq 0 3); do
    for i in $(seq 0 7); do
        echo h1D_${DERIVED_OBS}_centBinAll_ptBin${i}_${DATATYPE[h]} >> $1
    done

    for i in $(seq 0 6); do
        echo h1D_${DERIVED_OBS}_ptBinAll_hiBin${i}_${DATATYPE[h]} >> $1
    done
done

for j in $(seq 0 7); do
    for k in $(seq 0 6); do
        echo h1D_iaa_ptBin${j}_hiBin${k}_rebin >> $1
    done
done
