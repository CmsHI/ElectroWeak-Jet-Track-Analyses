#ifndef INPUTSETTINGS
#define INPUTSETTINGS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

class TrkSettings {
  public:
  //TrkSettings that will be set when a job number is specified
   int job;
   int nSkip;
   double ptMin, ptMax, centPUMin, centPUMax;

  //TrkSettings that should be the same across all processes
   std::string jobName;
   bool reuseSkim;
   bool checkClosure;
   int nPb, vz_window, nMC;
   bool doEff, doFake, doMult, doSecondary;

   bool doPthat, doVtx, doCentPU;
   std::vector<std::string> MCFiles;
   std::string DataFile;
   std::vector<double> pthatBins, pthatCrossSection;
   std::vector<int> multiRecoBins;

   int nPtBinCoarse, ptBinFine, etaBinFine, phiBinFine, centPUBinFine, jetBinFine;
   std::vector<double> ptBinCoarse;
   std::vector<int> nCentPUBinCoarse;
   std::vector<std::vector<double> > centPUBinCoarse, eventSkip;

   int highPurityDef;
   bool doCaloMatch, doTrackTriggerCuts, doOtherCut, doSplit;

   int nStep, fullIterations, terminateStep;
   std::vector<int> stepOrder;
   std::string jetDefinition;
   
   std::string trackTreeName;

   TrkSettings(std::string = "TrkCorrInputFile.txt");
};

TrkSettings::TrkSettings(std::string inputFile)
{
  std::cout << "Getting settings from file" << inputFile << std::endl;
  std::ifstream f;
  f.open (inputFile.c_str());

  //somewhat messy, but it just loads the input config file
  if (f.is_open())
  {
    //General options
    std::string Dump;
    getline(f,Dump);
    f >> jobName; getline(f,Dump);
    f >> reuseSkim; getline(f,Dump);
    f >> nPb; getline(f,Dump);
    f >> doEff; getline(f,Dump);
    f >> doFake; getline(f,Dump);
    f >> doMult; getline(f,Dump);
    f >> doSecondary; getline(f,Dump);
    f >> checkClosure; getline(f,Dump);
    getline(f,Dump);
    getline(f,Dump);

    //Event Cuts
    f >> vz_window; getline(f,Dump);
    getline(f,Dump);
    getline(f,Dump);
    
    //MC and reweighting options 
    f >> nMC; getline(f,Dump);
    for(int i = 0; i<nMC; i++)
    { 
      std::string tempstr;
      f >> tempstr; getline(f,Dump);
      MCFiles.push_back(tempstr);
    }
    getline(f,Dump);
    f >> doPthat; getline(f,Dump);
    for(int i = 0; i<nMC+1; i++)
    { 
      double temp;
      f >> temp;
      pthatBins.push_back(temp);
    }
    getline(f,Dump);
    for(int i = 0; i<nMC; i++)
    { 
      double temp;
      f >> temp;
      pthatCrossSection.push_back(temp);
    }
    getline(f,Dump);
    getline(f,Dump);
    f >> doVtx; getline(f,Dump);
    f >> doCentPU; getline(f,Dump);
    f >> DataFile; getline(f,Dump);
    getline(f,Dump);
    getline(f,Dump);   

    //Binnings
    f >> nPtBinCoarse; getline(f,Dump);
    for(int i = 0; i<nPtBinCoarse+1; i++)
    { 
      double temp;
      f >> temp;
      ptBinCoarse.push_back(temp);
    }
    getline(f,Dump);   
    getline(f,Dump);   
    for(int i = 0; i<nPtBinCoarse; i++)
    { 
      int temp;
      f >> temp;
      nCentPUBinCoarse.push_back(temp);
      getline(f,Dump);  
      std::vector<double> tempVec; 
      for(int j = 0; j<nCentPUBinCoarse.at(i)+1; j++)
      {
        double temp2;
        f >> temp2;
        tempVec.push_back(temp2);
      }
      centPUBinCoarse.push_back(tempVec);
      getline(f,Dump);   
    }
    getline(f,Dump);   
    for(int i = 0; i<nPtBinCoarse; i++)
    { 
      std::vector<double> tempVec; 
      for(int j = 0; j<nCentPUBinCoarse.at(i); j++)
      {
        double temp2;
        f >> temp2;
        tempVec.push_back(temp2);
      }
      eventSkip.push_back(tempVec);
      getline(f,Dump);   
    }
    getline(f,Dump);
    for(int i = 0; i<nPtBinCoarse; i++)
    { 
      int temp;
      f >> temp;
      multiRecoBins.push_back(temp);
    } 
    getline(f,Dump);   
    getline(f,Dump);   
    getline(f,Dump);    
    f >> ptBinFine; getline(f,Dump);
    f >> etaBinFine; getline(f,Dump);
    f >> phiBinFine; getline(f,Dump);
    f >> centPUBinFine; getline(f,Dump);
    f >> jetBinFine; getline(f,Dump);
    getline(f,Dump);   
    getline(f,Dump);   
    f >> highPurityDef; getline(f,Dump); 
    f >> doCaloMatch; getline(f,Dump); 
    f >> doTrackTriggerCuts; getline(f,Dump); 
    f >> doOtherCut; getline(f,Dump); 
    getline(f,Dump); 
    getline(f,Dump);  
    getline(f,Dump); 
    f >> nStep; getline(f,Dump); 
    for(int i=0; i<nStep; i++)
    {
      int temp; 
      f >> temp;
      stepOrder.push_back(temp);
    }
    getline(f,Dump);
    f >> fullIterations; getline(f,Dump); 
    f >> terminateStep; getline(f,Dump); 
    f >> jetDefinition;getline(f,Dump);
    f >> doSplit;   
    f.close();
    std::cout << "TrkSettings Loaded" << std::endl;

    //test reading in the input File
    std::cout << "Dumping TrkSettings" << std::endl;
    std::cout << jobName << std::endl;
    std::cout << nPb << " " << doEff << " " << doFake << " " << doMult << " " << doSecondary << std::endl; 
    std::cout << vz_window << std::endl;
    std::cout << nMC << std::endl;
    for(int i = 0; i<nMC; i++) std::cout << MCFiles.at(i) << std::endl;
    std::cout << doPthat << std::endl;
    for(int i = 0; i<nMC+1; i++) std::cout << pthatBins.at(i) << std::endl;
    for(int i = 0; i<nMC; i++) std::cout << pthatCrossSection.at(i) << std::endl;
    std::cout << doVtx << " " << DataFile << std::endl;
    std::cout << doCentPU << " " << DataFile << std::endl;
    std::cout << nPtBinCoarse << std::endl;
    for(int i = 0; i<nPtBinCoarse+1; i++) std::cout << ptBinCoarse.at(i) << std::endl;
    for(int i = 0; i<nPtBinCoarse; i++){
      std::cout << nCentPUBinCoarse.at(i) << " ";
      for(int j=0; j<nCentPUBinCoarse.at(i)+1; j++) std::cout << centPUBinCoarse.at(i).at(j) << " ";
      std::cout << std::endl;
    }
    for(int i = 0; i<nPtBinCoarse; i++){
      for(int j=0; j<nCentPUBinCoarse.at(i); j++) std::cout << eventSkip.at(i).at(j) << " ";
      std::cout << std::endl;
    }
    for(int i = 0; i<nPtBinCoarse; i++){
      std::cout << multiRecoBins.at(i)<< " ";
    }
    std::cout << std::endl;
    std::cout << ptBinFine << " " << etaBinFine << " " << phiBinFine << " " << centPUBinFine << " " << jetBinFine<<std::endl;
    std::cout << highPurityDef << " " << doCaloMatch << " " << doTrackTriggerCuts << " " << doOtherCut << std::endl;
    std::cout << nStep << std::endl;
    for(int i = 0; i<nStep; i++) std::cout << stepOrder.at(i);
    std::cout << std::endl;
    std::cout << fullIterations << " " << terminateStep << " " << jetDefinition << std::endl;
   

    if(nPb==0) trackTreeName = "ppTrack";
    if(nPb==2) trackTreeName = "anaTrack";
  }
  else
  {  
    std::cout << "Error! Cannot Find Input Configuration File!" << std::endl;
    exit(0);
  }
}

#endif
