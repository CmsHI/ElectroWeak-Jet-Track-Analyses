CXX = g++
ROOTFLAGS := `root-config --cflags --libs`
EXTRAFLAGS := -lTMVA -lRooFitCore -lRooFit
CXXFLAGS = -Wall -O2 -Werror -Wextra $(ROOTFLAGS) $(EXTRAFLAGS)
GCCVERSION := $(shell expr `gcc -dumpversion | cut -f1 -d.` \>= 6)
ifeq "$(GCCVERSION)" "1"
  CXXFLAGS += -Wno-error=misleading-indentation
endif
SRCS_C := $(wildcard */*.C)
PROGS_C := $(patsubst %.C,%.exe,$(SRCS_C))
SRCS_cc := $(wildcard */*/*.C)
PROGS_cc := $(patsubst %.C,%.exe,$(SRCS_cc))

all: $(PROGS_C) $(PROGS_cc)

%.exe: %.C
	$(CXX) $(CXXFLAGS)  -o $@ $<

%.exe: %.cc
	$(CXX) $(CXXFLAGS)  -o $@ $<

clean:
	rm */*.exe || true
	rm */*/*.exe || true
