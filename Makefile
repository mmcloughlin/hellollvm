LLVM_DIR=$(shell brew --prefix llvm)
LLVM_BIN=$(LLVM_DIR)/bin
LLVM_LIB=$(LLVM_DIR)/lib

CC=$(LLVM_BIN)/clang
CXX=$(CC)++
OPT=$(LLVM_BIN)/opt
OBJDUMP=$(LLVM_BIN)/llvm-objdump

CXXFLAGS  = -I$(LLVM_DIR)/include
CXXFLAGS += -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -Wno-long-long
CXXFLAGS += -fPIC -fvisibility-inlines-hidden
CXXFLAGS += -fno-exceptions -fno-rtti -std=c++11
CXXFLAGS += -Wall

LDFLAGS = -dynamiclib -Wl,-undefined,dynamic_lookup

passes = hello dump mutate rtlib
bin = $(addsuffix .out,$(passes))
dis = $(addsuffix .dis.s,$(passes))

all: $(bin) $(dis)

%.dylib: %.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.pc: %.cc
	$(CXX) -c -emit-llvm -o $@ $<

%.opt %.log: %.dylib example.pc
	$(OPT) -load $< -$* example.pc > $*.opt 2> $*.log

%.out: %.opt hook.o
	$(CXX) -O2 -o $@ $^

%.dis.s: %.out
	$(OBJDUMP) 	-disassemble-all $< > $@

clean:
	$(RM) *.dylib *.opt *.pc *.out *.o
