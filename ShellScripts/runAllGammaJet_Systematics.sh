#!/bin/bash
set -x

#DATE=$(date +"%Y-%m-%d_%H_%M_%S")
DATE=2016-08-15-Systematics

OUTDIR=/export/d00/scratch/luck/GAMMAJETFILES/${DATE}
mkdir -p $OUTDIR
H_INDIR=/mnt/hadoop/cms/store/user/luck/GAMMAJETFILES/${DATE}/
mkdir -p $H_INDIR

################ Minbias Skimming Section ####################
# echo "## minbias skimming"

# INFILE=./ShellScripts/PbPb_Data_MB_HiForest.list
# OUTFILE=PbPb_Data_minbiasJetSkim.root
# time ./ForestSkimmers/jets/minBiasJetSkim.exe ./CutConfigurations/gammaJet.conf $INFILE ${OUTDIR}/$OUTFILE > ${OUTDIR}/$OUTFILE.log &

# INFILE=/mnt/hadoop/cms/store/user/rbi/merged/Hydjet_Quenched_MinBias_5020GeV_750-HINPbPbWinter16DR-NoPU_75X_mcRun2_HeavyIon_forest_v2/0.root
# OUTFILE=PbPb_MC_minbiasJetSkim.root
# time ./ForestSkimmers/jets/minBiasJetSkim.exe ./CutConfigurations/gammaJet_mc.conf $INFILE ${OUTDIR}/$OUTFILE > ${OUTDIR}/$OUTFILE.log &

# wait

# cp ${OUTDIR}/*_minbiasJetSkim.root $H_INDIR/

####### Skimming, correcting, and histrogramming all done in Condor Job ###########
SAMPLE=(PbPb_Data PbPb_MC pp_Data pp_MC)
SYSTEMATIC=("" _JES_DOWN _JES_UP)
INFILE=(./ShellScripts/PbPb_Data_HiForest.list ./ShellScripts/PbPb_MC_HiForest.list /mnt/hadoop/cms/store/user/luck/2015-Data-promptRECO-photonSkims/pp-photonHLTFilter-v0-HiForest/0.root ./ShellScripts/pp_MC_HiForest.list)
NJOBS=(200 10 200 5)
MIXINGSAMPLE=($H_INDIR/PbPb_Data_minbiasJetSkim.root $H_INDIR/PbPb_MC_minbiasJetSkim.root DUMMY.root DUMMY.root)
CONFPOSTFIX1=("" _mc _pp _pp_mc)

# for ITERSAM in 0 1 2 3
# do
#     for ITERSYS in 0 1 2
#     do
#         CONFPOSTFIX=${CONFPOSTFIX1[ITERSAM]}${SYSTEMATIC[ITERSYS]}
#         mkdir -p $H_INDIR/${SAMPLE[ITERSAM]}${SYSTEMATIC[ITERSYS]}_unmerged/
#         ./ShellScripts/gammaJetSystematics_condor.sh ./CutConfigurations/gammaJet${CONFPOSTFIX}.conf ${INFILE[ITERSAM]} $H_INDIR/${SAMPLE[ITERSAM]}${SYSTEMATIC[ITERSYS]}_unmerged/ ${NJOBS[ITERSAM]} ${MIXINGSAMPLE[ITERSAM]}
#     done
# done

###########################################################


for ITERSAM in 0 1 2 3
do
    hadd $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetSkim_Corrected.root $H_INDIR/${SAMPLE[ITERSAM]}_unmerged/gammaJetSkim_Corrected*.root
done

for ITERSYS in 0 1 2
do
    for ITERSAM in 0 1 2 3
    do
        hadd $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYSTEMATIC[ITERSYS]}.root $H_INDIR/${SAMPLE[ITERSAM]}${SYSTEMATIC[ITERSYS]}_unmerged/gammaJetHistogram*.root
        CONFPOSTFIX=${CONFPOSTFIX1[ITERSAM]}${SYSTEMATIC[ITERSYS]}
        ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet${CONFPOSTFIX}.conf $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYSTEMATIC[ITERSYS]}.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root
    done
    ./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet${SYSTEMATIC[ITERSYS]}.conf ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/gammaJetHistograms${SYSTEMATIC[ITERSYS]}.root
done
