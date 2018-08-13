CXX = g++
CXXFLAGS += -Wall -Werror -Wextra -O2
ROOTFLAGS := `root-config --cflags --libs`
EXTRAFLAGS := -lTMVA -lRooFitCore -lRooFit

GCCVERSION := $(shell expr `gcc -dumpversion | cut -f1 -d.` \>= 6)
ifeq "$(GCCVERSION)" "1"
	CXXFLAGS += -Wno-error=misleading-indentation
endif

BUILD_DIR = ./build

NOCOMP=Performance/getTrigEff.C Performance/getTrigEff_fromInputHist.C Performance/getTrigEff_prescaleCorrected.C Performance/yj_eleMatchedPhotonZPeak_eleCut.C Histogramming/calcMCweights_usingSkimFile.C Histogramming/calcMCweights_fromInputHist.C Histogramming/calcMCweights.C Corrections/yj_photonEnergyCorrections.C Corrections/makeFinalCorrFile.C  
SRCS = $(wildcard */*.C)
SRCS += $(wildcard */*/*.C)
SRCS := $(filter-out $(NOCOMP), $(SRCS))
EXES = $(patsubst %.C,%.exe,$(SRCS))
DEPS = $(patsubst %.C,$(BUILD_DIR)/%.d,$(SRCS))

.PHONY: all clean

all: $(EXES)

%.exe: %.C
	@mkdir -p $(BUILD_DIR)/$(@D)
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(EXTRAFLAGS) -MMD -MF $(BUILD_DIR)/$(@D)/$(*F).d $< -o $@

clean:
	@$(RM) $(EXES) $(DEPS)
	@rm -rf $(BUILD_DIR)/*

-include $(DEPS)
