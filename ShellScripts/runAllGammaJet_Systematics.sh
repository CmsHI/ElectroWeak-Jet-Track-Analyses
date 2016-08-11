#!/bin/bash
set -x

#DATE=$(date +"%Y-%m-%d_%H_%M_%S")
DATE=2016-08-11-Systematics

OUTDIR=/export/d00/scratch/luck/GAMMAJETFILES/${DATE}
mkdir -p $OUTDIR
H_INDIR=/mnt/hadoop/cms/store/user/luck/GAMMAJETFILES/${DATE}/
mkdir -p $H_INDIR

################ Minbias Skimming Section ####################
echo "## minbias skimming"

INFILE=./ShellScripts/PbPb_Data_MB_HiForest.list
OUTFILE=PbPb_Data_minbiasJetSkim.root
time ./ForestSkimmers/jets/minBiasJetSkim.exe ./CutConfigurations/gammaJet.conf $INFILE ${OUTDIR}/$OUTFILE > ${OUTDIR}/$OUTFILE.log &

INFILE=/mnt/hadoop/cms/store/user/rbi/merged/Hydjet_Quenched_MinBias_5020GeV_750-HINPbPbWinter16DR-NoPU_75X_mcRun2_HeavyIon_forest_v2/0.root 
OUTFILE=PbPb_MC_minbiasJetSkim.root
time ./ForestSkimmers/jets/minBiasJetSkim.exe ./CutConfigurations/gammaJet_mc.conf $INFILE ${OUTDIR}/$OUTFILE > ${OUTDIR}/$OUTFILE.log &

wait

mv ${OUTDIR}/*_minbiasJetSkim.root $H_INDIR/

####### Skimming, correcting, and histrogramming all done in Condor Job ###########
SAMPLE=(PbPb_Data PbPb_MC pp_Data pp_MC)
SYSTEMATIC=(JER JES_DOWN JES_UP RESIDUAL)
INFILE=(./ShellScripts/PbPb_Data_HiForest.list ./ShellScripts/PbPb_MC_HiForest.list /mnt/hadoop/cms/store/user/luck/2015-Data-promptRECO-photonSkims/pp-photonHLTFilter-v0-HiForest/0.root ./ShellScripts/pp_MC_HiForest.list)
NJOBS=(200 10 200 5)
MIXINGSAMPLE=($H_INDIR/PbPb_Data_minbiasJetSkim.root $H_INDIR/PbPb_MC_minbiasJetSkim.root DUMMY.root DUMMY.root)
CONFPOSTFIX1=("" _mc _pp _pp_mc)

for ITERSAM in 0 1 2 3
do
    for ITERSYS in 0 1 2 3
    do
	CONFPOSTFIX=${CONFPOSTFIX1[ITERSAM]}_${SYSTEMATIC[ITERSYS]}
	mkdir -p $H_INDIR/${SAMPLE[ITERSAM]}_${SYSTEMATIC[ITERSYS]}_unmerged/
	./ShellScripts/gammaJetSystematics_condor.sh ./CutConfigurations/gammaJet${CONFPOSTFIX}.conf ${INFILE[ITERSAM]} $H_INDIR/${SAMPLE[ITERSAM]}_${SYSTEMATIC[ITERSYS]}_unmerged/ ${NJOBS[ITERSAM]} ${MIXINGSAMPLE[ITERSAM]}
    done
done

# ######## Histogram Arithmetic Section ##############
# echo "## Arithmetic"
# time ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetHistogram.root ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic.root > ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic.root.log &

# time ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet_mc.conf ${OUTDIR}/PbPb_MC_gammaJetHistogram.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic.root > ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic.root.log &

# time ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet_pp.conf ${OUTDIR}/pp_Data_gammaJetHistogram.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic.root > ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic.root.log &

# time ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet_pp_mc.conf ${OUTDIR}/pp_MC_gammaJetHistogram.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic.root > ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic.root.log &

# wait

# ######## IAA Section ##############
# time ./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic.root ${OUTDIR}/gammaJetHistograms.root > ${OUTDIR}/gammaJetHistograms.root.log &

# wait
