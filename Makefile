CXX = g++
ROOTFLAGS = `root-config --cflags --libs`
TMVAFLAGS = -lTMVA
CXXFLAGS = -Wall -g -Werror -Wextra $(ROOTFLAGS) $(TMVAFLAGS)
SRCS_C = $(wildcard */*.C)
PROGS_C = $(patsubst %.C,%.exe,$(SRCS_C))
SRCS_cc = $(wildcard */*/*.C)
PROGS_cc = $(patsubst %.C,%.exe,$(SRCS_cc))

all: $(PROGS_C) $(PROGS_cc)

%.exe: %.C
	$(CXX) $(CXXFLAGS)  -o $@ $<

%.exe: %.cc
	$(CXX) $(CXXFLAGS)  -o $@ $<

clean:
	rm */*.exe
	rm */*/*.exe
