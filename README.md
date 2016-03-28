# ElectroWeak-Jet-Track-Analyses

This repository holds the code for Z+jets, gamma-jet, gamma-track, and other similarly styled analyses for the CMS Heavy Ion collaboration.

This repository is larger and more complicated than past projects, but is expected to be used for multiple analyses, re-using the same code and settings between analyses.

## Analysis structure

The basic analysis workflow is

1. Using the code in the *ForestSkimmers* folder, take an input HiForest file and make a **skim** of the objects you want to study (Zs, photons, jets, or tracks).
2. Use the code in the *Corrections* folder to apply the appropriate set of corrections and/or cuts to the **skim** files and produce a **FullyCorrectedObjectSkim**
3. Use the code in the *CorrelationTuple* folder to then combine two **FullyCorrectedObjectSkim** from the previous step into a single **CorrelationTuple** .
4. Use the code in the *Histogramming* folder to make the physics observables root files from the **CorrelationTuple**s.
5. Use the code in the *Plotting* folder to produce the final publish-quality plots from the root files. This is the step where PbPb/pp division is performed, if necessary.

All cuts applied at every step come from files in the *CutConfigurations* folder. All possible cuts are set at run-time using those files.

## Coding guidelines

### General

- Do not use strings as labels. If at all possible, use C++ enumerates
- All code should compile with g++ with no warnings and run in "batch mode". Every macro should take a CutConfigurations file as the first commandline argument. Makefiles will be provided to facilitate compilation.
- All possible cut values should be included in the CutConfigurations object to be used at *run time* NOT compile time. There will be a namespace of cuts for each step, i.e.
  ```C++
  skim.photon_eta_cut=3.0
  corrections.photon_eta_cut=2.8
  histogramming.photon_eta_cut=2.4
  ...
  ```
  It is expected that there will be one CutConfigurations file for the "golden" set of cuts, and then several files with different cuts meant for Systematic Uncertainty studies and testing. Make sure these are clearly labelled.
- All steps should be able to be performed on personal laptops, ESPECIALLY the plotting step.

### ForestSkimmers

- Each physics object should get it's own ForestSkimmer, i.e. one for photons, one for Zs, one for jets, etc.
- Minimize the number of cuts applied at this step in order to make systematic studies easier later. We aim to have this step run only once per HiForest.
- The headers for necessary HiForest TTrees should go in the *TreeHeaders* folder.
- Make absolutely sure that the Event, Run, and LumiBlock are stored in the output TTree.

### Corrections

- This step is where the single-object cuts should be applied (i.e. photon eta, Z mass).
- Corrections such as the photon energy scale and tracking efficiency should be applied at this step.
- If necessary, root files containing correction tables should be uploaded to this folder. These should be kept small.


### CorrelationTuple

- Use the EventMatchingCMS class to match events between two *FullyCorrectedObject* trees. In general this process should be fast and simple if both inputs start from the same HiForest.
- No cuts should be applied at this step.
- In general, the output of this step should be a simple combination of two FullyCorrectedObject TTrees in the same file, with event numbers lined up in the TTrees.

### Histogramming

- Any cuts that operate on two-object combinations (i.e. photon-jet dPhi) should occur at this step.
- This is where the meat of most analyses will happen, and where the least amount of code can be re-used. For example, minbias-mixing background subtraction would occur at this step.

### Plotting

- This step should produce the publication-quality plots based on the histogramming output.
- Make sure that any 'rootlogon.C' macros necessary for font sizes etc. are explicitly loaded in any macro, do not depend on users manually loading these settings.
- It is especially important that this step is able to be performed on personal laptops.
- It is especially important that this step is able to be performed in "batch mode" for reproducibility.

### Performance

 - This folder is intended for performance studies on different physics objects and observables. These include quick checks such as turn on curves, energy scale plots, object spectra and also more comprehensive studies such as Data vs. MC comparison, release validation etc.
 - This is not a direct part of the analysis workflow, but serves as a cross-check for analysis.
 - The main purpose of macros in this folder is to work on one or more input HiForest files and output a set of histograms to a ".root" file. Saving output as "png", "pdf" or other graphics file can be optional. However, it is discouraged to output only graphics files, but no ".root" file. For example, photonTurnOn.C outputs a ".root" file with photon turn on curves and if the related option is specified, then a graphics file is also generated.
 - When saving a graphics file, it is encouraged to save the file also in ".C" format so that any necessary small modification can be done quickly.

### Configurations

 - This folder is intended to store configuration files for different performance and analysis studies.
 - It is recommended to follow a sound organization when placing the ".conf" files into that folder. For example "Configurations/zJet" should contain configurations related to z-Jet correlations analysis.

