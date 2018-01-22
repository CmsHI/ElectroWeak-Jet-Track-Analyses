### Creating Lists

Run the following to create the lists for Pythia8_Photon*_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST :

  ```bash
  cd $EWJTAHOME # go to directory where the repository is based
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_Photon15_pp502_TuneCUETP8M1/HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3*-v1-FOREST/0.root > Configurations/filelists/Pythia8_Photon15_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_Photon30_pp502_TuneCUETP8M1/HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3*-v1-FOREST/0.root > Configurations/filelists/Pythia8_Photon30_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_Photon50_pp502_TuneCUETP8M1/HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3*-v1-FOREST/0.root > Configurations/filelists/Pythia8_Photon50_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_Photon80_pp502_TuneCUETP8M1/HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3*-v1-FOREST/0.root > Configurations/filelists/Pythia8_Photon80_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_Photon120_pp502_TuneCUETP8M1/HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3*-v1-FOREST/0.root > Configurations/filelists/Pythia8_Photon120_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_Photon*_pp502_TuneCUETP8M1/HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3*-v1-FOREST/0.root > Configurations/filelists/Pythia8_PhotonXX_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list
  ```
Run the following to create the lists for Pythia8_AllQCDPhoton*_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST :

  ```bash
  cd $EWJTAHOME # go to directory where the repository is based
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton15_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/180109_103013/0000/*.root /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton15_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14_ext1-v1-FOREST/180108_211712/0000/*.root > Configurations/filelists/Pythia8_AllQCDPhoton15_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/180109_103036/0000/HiForestAOD_*.root /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14_ext1-v1-FOREST/180108_211735/0000/*.root > Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton50_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/180109_103059/0000/HiForestAOD_*.root /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton50_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14_ext1-v1-FOREST/180108_211758/0000/HiForestAOD_*.root > Configurations/filelists/Pythia8_AllQCDPhoton50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton80_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/180109_103121/0000/HiForestAOD_*.root /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton80_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14_ext1-v1-FOREST/180109_151514/0000/HiForestAOD_*.root > Configurations/filelists/Pythia8_AllQCDPhoton80_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton120_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/180109_103146/0000/HiForestAOD_*.root /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton120_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14_ext1-v1-FOREST/180109_151428/0000/HiForestAOD_*.root > Configurations/filelists/Pythia8_AllQCDPhoton120_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  rm Configurations/filelists/Pythia8_AllQCDPhotonXX_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  cat Configurations/filelists/Pythia8_AllQCDPhoton15_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list >> Configurations/filelists/Pythia8_AllQCDPhotonXX_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  cat Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list >> Configurations/filelists/Pythia8_AllQCDPhotonXX_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  cat Configurations/filelists/Pythia8_AllQCDPhoton50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list >> Configurations/filelists/Pythia8_AllQCDPhotonXX_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  cat Configurations/filelists/Pythia8_AllQCDPhoton80_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list >> Configurations/filelists/Pythia8_AllQCDPhotonXX_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  cat Configurations/filelists/Pythia8_AllQCDPhoton120_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list >> Configurations/filelists/Pythia8_AllQCDPhotonXX_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  ```

Run the following to create the lists for Pythia8_AllQCDPhoton*Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST :

  ```bash
  cd $EWJTAHOME # go to directory where the repository is based
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton15Flt30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/180109_102718/0000/*.root /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton15Flt30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14_ext1-v1-FOREST/180108_161438/0000/*.root > Configurations/filelists/Pythia8_AllQCDPhoton15Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton30Flt30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/180109_102740/0000/*.root /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton30Flt30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14_ext1-v1-FOREST/180108_211216/0000/*.root > Configurations/filelists/Pythia8_AllQCDPhoton30Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton50Flt30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/180109_102806/0000/*.root /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton50Flt30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14_ext1-v1-FOREST/180108_211239/0000/*.root > Configurations/filelists/Pythia8_AllQCDPhoton50Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton80Flt30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/180109_102840/0000/*.root /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton80Flt30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14_ext1-v1-FOREST/180108_211303/0000/*.root > Configurations/filelists/Pythia8_AllQCDPhoton80Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton120Flt30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/180109_102904/0000/*.root /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton120Flt30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14_ext1-v1-FOREST/180109_152019/0000/*.root > Configurations/filelists/Pythia8_AllQCDPhoton120Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  rm Configurations/filelists/Pythia8_AllQCDPhotonXXFlt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  cat Configurations/filelists/Pythia8_AllQCDPhoton15Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list >> Configurations/filelists/Pythia8_AllQCDPhotonXXFlt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  cat Configurations/filelists/Pythia8_AllQCDPhoton30Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list >> Configurations/filelists/Pythia8_AllQCDPhotonXXFlt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  cat Configurations/filelists/Pythia8_AllQCDPhoton50Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list >> Configurations/filelists/Pythia8_AllQCDPhotonXXFlt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  cat Configurations/filelists/Pythia8_AllQCDPhoton80Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list >> Configurations/filelists/Pythia8_AllQCDPhotonXXFlt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  cat Configurations/filelists/Pythia8_AllQCDPhoton120Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list >> Configurations/filelists/Pythia8_AllQCDPhotonXXFlt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  ```

Run the following to create the lists for Pythia8_AllQCDPhoton*Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST :

  ```bash
  cd $EWJTAHOME # go to directory where the repository is based
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton30Flt50_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/180111_184718/0000/*.root > Configurations/filelists/Pythia8_AllQCDPhoton30Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton50Flt50_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/180111_184802/0000/*.root > Configurations/filelists/Pythia8_AllQCDPhoton50Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton80Flt50_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/180111_184839/0000/*.root > Configurations/filelists/Pythia8_AllQCDPhoton80Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  ls -1 /mnt/hadoop/cms/store/user/katatar/official/Pythia8_AllQCDPhoton120Flt50_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/180111_184934/0000/*.root > Configurations/filelists/Pythia8_AllQCDPhoton120Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  rm Configurations/filelists/Pythia8_AllQCDPhotonXXFlt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  cat Configurations/filelists/Pythia8_AllQCDPhoton30Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list >> Configurations/filelists/Pythia8_AllQCDPhotonXXFlt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  cat Configurations/filelists/Pythia8_AllQCDPhoton50Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list >> Configurations/filelists/Pythia8_AllQCDPhotonXXFlt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  cat Configurations/filelists/Pythia8_AllQCDPhoton80Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list >> Configurations/filelists/Pythia8_AllQCDPhotonXXFlt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  cat Configurations/filelists/Pythia8_AllQCDPhoton120Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list >> Configurations/filelists/Pythia8_AllQCDPhotonXXFlt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list
  ```
