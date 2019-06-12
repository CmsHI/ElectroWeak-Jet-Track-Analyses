CXX = g++
CXXFLAGS += -Wall -Werror -Wextra -O2
ROOTFLAGS := `root-config --cflags --libs`
EXTRAFLAGS := -lTMVA -lXMLIO -lTMVAGui -lRooFitCore -lRooFit

GCCVERSION := $(shell expr `gcc -dumpversion | cut -f1 -d.` \>= 6)
ifeq "$(GCCVERSION)" "1"
	CXXFLAGS += -Wno-error=misleading-indentation
endif

BUILD_DIR = ./build

SRCS = $(wildcard */*.C)
SRCS += $(wildcard */*/*.C)
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
