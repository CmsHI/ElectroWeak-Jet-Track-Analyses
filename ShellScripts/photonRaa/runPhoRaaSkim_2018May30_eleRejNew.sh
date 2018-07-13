#!/bin/bash
set -x

#DATE=$(date +"%Y-%m-%d_%H_%M_%S")
DATE=2018-04-06-nominal

OUTDIR=/u/user/goyeonju/scratch/files/photons2016/GAMMAJETFILES/${DATE}

# ############ SKIMMING SECTION ##############
echo "## Skimming"

mkdir -p $OUTDIR

#INFILE=./ShellScripts/photonRaa/pp_EmEnrMC_HiForest_v1.list
#OUTMCSKIM=pp_EmEnrMC_photonRaaSkim_GED_tmva_test.root
#CONFIG=./CutConfigurations/photonRaa_pp_mc_EmEnr_tmva_test.conf
#time ./ForestSkimmers/photons/photonRaaSkim_workingOn_pbpbGED_ppGED.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} 1 > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
#
#echo "### pp DATA GED ###"
#INFILE=./ShellScripts/photonRaa/pp_Data_HiForest.list
#OUTMCSKIM=pp_Data_photonRaaSkim_GED.root
#CONFIG=./CutConfigurations/photonRaa_pp_tmva.conf
#time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
#
echo "### pp MC Island ###"
INFILE=./ShellScripts/photonRaa/pp_MC_HiForest_v1.list
OUTMCSKIM=pp_MC_photonRaaSkim_eleRejNew.root
CONFIG=./CutConfigurations/photonRaa_pp_mc.conf
time ./ForestSkimmers/photons/photonRaaSkim_workingOn_eleRejNew.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

INFILE=./ShellScripts/photonRaa/pp_EmEnrMC_HiForest_v1.list
OUTMCSKIM=pp_EmEnrMC_photonRaaSkim_eleRejNew.root
CONFIG=./CutConfigurations/photonRaa_pp_mc_EmEnr.conf
time ./ForestSkimmers/photons/photonRaaSkim_workingOn_eleRejNew.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} 1 > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

echo "### PbPb MC Island ###"
INFILE=./ShellScripts/photonRaa/PbPb_MC_HiForest_v14.list
OUTMCSKIM=PbPb_MC_photonRaaSkim_v14_eleRejNew.root
CONFIG=./CutConfigurations/photonRaa_mc.conf
time ./ForestSkimmers/photons/photonRaaSkim_workingOn_eleRejNew.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

INFILE=./ShellScripts/photonRaa/PbPb_EmEnrMC_HiForest_v14.list
OUTMCSKIM=PbPb_EmEnrMC_photonRaaSkim_v14_eleRejNew.root
CONFIG=./CutConfigurations/photonRaa_mc_EmEnr.conf
time ./ForestSkimmers/photons/photonRaaSkim_workingOn_eleRejNew.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} 1 > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

#echo "### pp MC GED ###"
#INFILE=./ShellScripts/photonRaa/pp_MC_HiForest_v1.list
#OUTMCSKIM=pp_MC_photonRaaSkim_GED_eleRejNew.root
#CONFIG=./CutConfigurations/photonRaa_pp_mc_tmva.conf
#time ./ForestSkimmers/photons/photonRaaSkim_workingOn_eleRejNew.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
#
#INFILE=./ShellScripts/photonRaa/pp_EmEnrMC_HiForest_v1.list
#OUTMCSKIM=pp_EmEnrMC_photonRaaSkim_GED_eleRejNew.root
#CONFIG=./CutConfigurations/photonRaa_pp_mc_EmEnr_tmva.conf
#time ./ForestSkimmers/photons/photonRaaSkim_workingOn_eleRejNew.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} 1 > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
#
#echo "### PbPb MC GED ###"
#INFILE=./ShellScripts/photonRaa/PbPb_MC_HiForest_v14.list
#OUTMCSKIM=PbPb_MC_photonRaaSkim_v14_GED_eleRejNew.root
#CONFIG=./CutConfigurations/photonRaa_mc_tmva.conf
#time ./ForestSkimmers/photons/photonRaaSkim_workingOn_eleRejNew.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
#
#INFILE=./ShellScripts/photonRaa/PbPb_EmEnrMC_HiForest_v14.list
#OUTMCSKIM=PbPb_EmEnrMC_photonRaaSkim_v14_GED_eleRejNew.root
#CONFIG=./CutConfigurations/photonRaa_mc_EmEnr_tmva.conf
#time ./ForestSkimmers/photons/photonRaaSkim_workingOn_eleRejNew.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} 1 > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
