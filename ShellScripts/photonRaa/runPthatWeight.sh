#!/bin/bash
set -x

#echo "### PbPb AllQCD Drum ###"
#INFILE=./ShellScripts/photonRaa/PbPb/PbPb_MC_Drum_HiForest.list
#INFILE_L=./ShellScripts/photonRaa/PbPb/PbPb_MC_Drum_HiForest_lowest.list
#time ./Corrections/ptHatWeights.exe ${INFILE} ${INFILE_L}> ./log/ptHatWeight_PbPb_MC_Drum.log || echo "ERROR"
#
#echo "### PbPb AllQCD Drum ext1 ###"
#INFILE=./ShellScripts/photonRaa/PbPb/PbPb_MC_Drum_ext1_HiForest.list
#INFILE_L=./ShellScripts/photonRaa/PbPb/PbPb_MC_Drum_ext1_HiForest_lowest.list
#time ./Corrections/ptHatWeights.exe ${INFILE} ${INFILE_L}> ./log/ptHatWeight_PbPb_MC_Drum_ext1.log || echo "ERROR"
#
#echo "### PbPb AllQCD Drum v1+ext1 ###"
#INFILE=./ShellScripts/photonRaa/PbPb/PbPb_MC_Drum_v1_and_ext1_HiForest.list
#INFILE_L=./ShellScripts/photonRaa/PbPb/PbPb_MC_Drum_v1_and_ext1_HiForest_lowest.list
#time ./Corrections/ptHatWeights.exe ${INFILE} ${INFILE_L}> ./log/ptHatWeight_PbPb_MC_Drum_v1_and_ext1.log || echo "ERROR"

#echo "### PbPb AllQCD Cymbal ###"
#INFILE=./ShellScripts/photonRaa/PbPb/PbPb_MC_Cymbal_HiForest.list
#INFILE_L=./ShellScripts/photonRaa/PbPb/PbPb_MC_Cymbal_HiForest_lowest.list
#time ./Corrections/ptHatWeights.exe ${INFILE} ${INFILE_L}> ./log/ptHatWeight_PbPb_MC_Cymbal.log || echo "ERROR"
#
#echo "### PbPb AllQCD Cymbal Flt30 ###"
#INFILE=./ShellScripts/photonRaa/PbPb/PbPb_MC_Cymbal_Flt30_HiForest.list
#INFILE_L=./ShellScripts/photonRaa/PbPb/PbPb_MC_Cymbal_Flt30_HiForest_lowest.list
#time ./Corrections/ptHatWeights.exe ${INFILE} ${INFILE_L}> ./log/ptHatWeight_PbPb_MC_Cymbal_Flt30.log || echo "ERROR"
#
echo "### PbPb AllQCD Cymbal + ext ###"
INFILE=./ShellScripts/photonRaa/PbPb/PbPb_MC_Cymbal_HiForest.list
INFILE_L=./ShellScripts/photonRaa/PbPb/PbPb_MC_Cymbal_HiForest_lowest.list
time ./Corrections/ptHatWeights.exe ${INFILE} ${INFILE_L}> ./log/ptHatWeight_PbPb_MC_Cymbal.log || echo "ERROR"
#
echo "### pp AllQCD + ext ###"
INFILE=./ShellScripts/photonRaa/pp/pp_MC_HiForest_v1.list
INFILE_L=./ShellScripts/photonRaa/pp/pp_MC_HiForest_v1_lowest.list
time ./Corrections/ptHatWeights.exe ${INFILE} ${INFILE_L}> ./log/ptHatWeight_pp.log || echo "ERROR"
#
#echo "### pp AllQCD ###"
#INFILE=./ShellScripts/photonRaa/pp/pp_MC_HiForest_v1.list
#INFILE_L=./ShellScripts/photonRaa/pp/pp_MC_HiForest_v1_lowest.list
#time ./Corrections/ptHatWeights.exe ${INFILE} ${INFILE_L}> ./log/ptHatWeight_pp.log || echo "ERROR"
#
#echo "### PbPb EmEnr Drum ###"
#INFILE=./ShellScripts/photonRaa/PbPb/PbPb_EmEnrMC_Drum_HiForest.list
#INFILE_L=./ShellScripts/photonRaa/PbPb/PbPb_EmEnrMC_Drum_HiForest_lowest.list
#time ./Corrections/ptHatWeights_EmEnr.exe ${INFILE} ${INFILE_L}> ./log/ptHatWeight_PbPb_EmEnrMC_Drum.log || echo "ERROR"
#
#echo "### PbPb EmEnr Cymbal ###"
#INFILE=./ShellScripts/photonRaa/PbPb/PbPb_EmEnrMC_Cymbal_HiForest.list
#INFILE_L=./ShellScripts/photonRaa/PbPb/PbPb_EmEnrMC_Cymbal_HiForest_lowest.list
#time ./Corrections/ptHatWeights_EmEnr.exe ${INFILE} ${INFILE_L}> ./log/ptHatWeight_PbPb_EmEnrMC_Cymbal.log || echo "ERROR"
#
#echo "### pp EmEnr ###"
#INFILE=./ShellScripts/photonRaa/pp/pp_EmEnrMC_HiForest_v1.list
#INFILE_L=./ShellScripts/photonRaa/pp/pp_EmEnrMC_HiForest_v1_lowest.list
#time ./Corrections/ptHatWeights_EmEnr.exe ${INFILE} ${INFILE_L}> ./log/ptHatWeight_pp_EmEnr.log || echo "ERROR"

#ShellScripts/photonRaa/PbPb/PbPb_EmEnrMC_Drum_HiForest_lowest.list
#ShellScripts/photonRaa/PbPb/PbPb_MC_Drum_v1_and_ext1_HiForest_lowest.list
#ShellScripts/photonRaa/PbPb/PbPb_MC_Cymbal_Flt30_HiForest_lowest.list
#ShellScripts/photonRaa/PbPb/PbPb_MC_Cymbal_v1_and_Flt30_HiForest_lowest.list
#ShellScripts/photonRaa/PbPb/PbPb_EmEnrMC_Cymbal_HiForest_lowest.list
#ShellScripts/photonRaa/PbPb/PbPb_MC_Cymbal_HiForest_lowest.list
#ShellScripts/photonRaa/PbPb/PbPb_MC_Drum_HiForest_lowest.list
#ShellScripts/photonRaa/PbPb/PbPb_MC_Drum_ext1_HiForest_lowest.list
#ShellScripts/photonRaa/pp/pp_MC_HiForest_v1_lowest.list
#ShellScripts/photonRaa/pp/pp_EmEnrMC_HiForest_v1_lowest.list
