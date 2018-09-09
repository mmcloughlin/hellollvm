LLVM_DIR=$(shell brew --prefix llvm)
LLVM_BIN=$(LLVM_DIR)/bin

CC=$(LLVM_BIN)/clang
CXX=$(CC)++
OPT=$(LLVM_BIN)/opt

CXXFLAGS  = -I$(LLVM_DIR)/include
CXXFLAGS += -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -Wno-long-long
CXXFLAGS += -fPIC -fvisibility-inlines-hidden
CXXFLAGS += -fno-exceptions -fno-rtti -std=c++11
CXXFLAGS += -Wall

LDFLAGS = -dynamiclib -Wl,-undefined,dynamic_lookup

all: hello.log

%.dylib: %.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.pc: %.cc
	$(CXX) $(CXXFLAGS) -c -emit-llvm -o $@ $<

%.opt %.log: %.dylib example.pc
	$(OPT) -load $< -$* example.pc > $*.opt 2> $*.log

clean:
	$(RM) *.dylib *.opt *.pc
