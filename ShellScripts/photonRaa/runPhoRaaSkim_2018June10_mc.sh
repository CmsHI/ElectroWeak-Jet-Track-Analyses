#!/bin/bash
set -x

#DATE=$(date +"%Y-%m-%d_%H_%M_%S")
DATE=2018-06-10-nominal

OUTDIR=/u/user/goyeonju/scratch/files/photons2016/GAMMAJETFILES/${DATE}

# ############ SKIMMING SECTION ##############
echo "## Skimming"

mkdir -p $OUTDIR

#echo "### pp DATA ###"
#INFILE=./ShellScripts/photonRaa/pp/pp_Data_HiForest.list
#OUTMCSKIM=pp_Data_photonRaaSkim.root
#CONFIG=./CutConfigurations/photonRaa_pp.conf
#time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
#
#echo "### PbPb DATA ###"
#INFILE=./ShellScripts/photonRaa/PbPb/PbPb_Data_HiForest.list
#OUTMCSKIM=PbPb_Data_photonRaaSkim.root
#CONFIG=./CutConfigurations/photonRaa.conf
#time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
#
echo "### pp MC AllQCDPhotons ###"
INFILE=./ShellScripts/photonRaa/pp/pp_MC_HiForest_v1.list
OUTMCSKIM=pp_MC_photonRaaSkim.root
CONFIG=./CutConfigurations/photonRaa_pp_mc.conf
time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

echo "### PbPb MC AllQCDPhotons Cymbal noFlt30 ###"
INFILE=./ShellScripts/photonRaa/PbPb/PbPb_MC_Cymbal_HiForest.list
OUTMCSKIM=PbPb_MC_photonRaaSkim_Cymbal_noFlt30.root
CONFIG=./CutConfigurations/photonRaa_mc.conf
time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

echo "### PbPb MC AllQCDPhotons Drum v1+ext1 ###"
INFILE=./ShellScripts/photonRaa/PbPb/PbPb_MC_Drum_v1_and_ext1_HiForest.list
OUTMCSKIM=PbPb_MC_photonRaaSkim_Drum_v1_and_ext1.root
CONFIG=./CutConfigurations/photonRaa_mc_Drum_v1_and_ext1.conf
time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"


echo "### pp MC EmEnrichedDijet ###"
INFILE=./ShellScripts/photonRaa/pp/pp_EmEnrMC_HiForest_v1.list
OUTMCSKIM=pp_EmEnrMC_photonRaaSkim.root
CONFIG=./CutConfigurations/photonRaa_pp_mc_EmEnr.conf
time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} 1 > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

echo "### PbPb MC EmEnriched Cymbal ###"
INFILE=./ShellScripts/photonRaa/PbPb/PbPb_EmEnrMC_Cymbal_HiForest.list
OUTMCSKIM=PbPb_EmEnrMC_photonRaaSkim_Cymbal.root
CONFIG=./CutConfigurations/photonRaa_mc_EmEnr.conf
time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} 1 > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

echo "### PbPb MC EmEnriched Drum ###"
INFILE=./ShellScripts/photonRaa/PbPb/PbPb_EmEnrMC_Drum_HiForest.list
OUTMCSKIM=PbPb_EmEnrMC_photonRaaSkim_Drum.root
CONFIG=./CutConfigurations/photonRaa_mc_EmEnr_Drum.conf
time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} 1 > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
