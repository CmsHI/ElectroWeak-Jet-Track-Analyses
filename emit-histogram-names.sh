#!/bin/bash

[ -f $1 ] && rm $1

BASE_OBS=(xjg dphi ptJet)

for i in $(seq 0 2); do
    for j in $(seq 0 7); do
        for k in $(seq 0 6); do
            echo h1D_${BASE_OBS[i]}_ptBin${j}_hiBin${k}_phoSIG_jetSIG_final_norm >> $1
        done
    done
done

XJG_BASED=(xjg_mean rjg)
DPHI_BASED=(dphi_width dphi_pedestal)

OBS_SET=(XJG_BASED DPHI_BASED)
OBS_SUFFIX=(_phoSIG_jetSIG "")

for n in $(seq 0 1); do
    OBS_NAME=${OBS_SET[n]}[i]

    for i in $(seq 0 1); do
        for j in $(seq 0 7); do
            echo h1D_${!OBS_NAME}_centBinAll_ptBin${j}${OBS_SUFFIX[n]} >> $1
        done

        for j in $(seq 0 6); do
            echo h1D_${!OBS_NAME}_ptBinAll_hiBin${j}${OBS_SUFFIX[n]} >> $1
        done
    done
done
