#!/bin/bash
set -x

#DATE=$(date +"%Y-%m-%d_%H_%M_%S")
DATE=2017--nominal

OUTDIR=/u/user/goyeonju/scratch/files/photons2016/GAMMAJETFILES/${DATE}
#OUTDIR=/u/user/goyeonju/PhotonAnalysis2016/GAMMAJETFILES/${DATE}

# ############ SKIMMING SECTION ##############
echo "## Skimming"

mkdir -p $OUTDIR

# echo "### PbPb Data/MC ###"

INFILE=./ShellScripts/photonRaa/PbPb_MC_HiForest_Flt30.list
OUTMCSKIM=PbPb_MC_photonRaaSkim_Flt30.root
time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ./CutConfigurations/photonRaa_mc.conf ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

#INFILE=./ShellScripts/photonRaa/PbPb_EmEnrMC_HiForest_v14.list
#OUTMCSKIM=PbPb_EmEnrMC_photonRaaSkim_v14.root
#time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ./CutConfigurations/photonRaa_mc_EmEnr.conf ${INFILE} ${OUTDIR}/${OUTMCSKIM} 1 > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
#
INFILE=./ShellScripts/photonRaa/PbPb_MC_HiForest_v14.list
OUTMCSKIM=PbPb_MC_photonRaaSkim_v14.root
time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ./CutConfigurations/photonRaa_mc_noFlt30.conf ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

echo "### pp Data/MC ###"

#INFILE=ShellScripts/gammaJet/pp_Data_HiForest.list
#OUTFILE=pp_Data_photonRaaSkim.root
#time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ./CutConfigurations/photonRaa_pp.conf ${INFILE} ${OUTDIR}/${OUTFILE} > ${OUTDIR}/${OUTFILE}.log || echo "ERROR"

#INFILE=./ShellScripts/gammaJet/pp_MC_HiForest_knu.list
#OUTMCSKIM=test_pp_MC.root
#time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ./CutConfigurations/photonRaa_pp_mc.conf $INFILE ${OUTDIR}/${OUTMCSKIM}  > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

#INFILE=./ShellScripts/gammaJet/pp_EmEnrMC_HiForest_knu.list
#OUTMCSKIM=pp_EmEnrMC_photonRaaSkim.root
#time ./ForestSkimmers/photons/photonRaaSkim_workingOn.exe ./CutConfigurations/photonRaa_pp_mc_EmEnr.conf $INFILE ${OUTDIR}/${OUTMCSKIM} 1 > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
