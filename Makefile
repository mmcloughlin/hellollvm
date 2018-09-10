LLVM_DIR=$(shell brew --prefix llvm)
LLVM_BIN=$(LLVM_DIR)/bin
LLVM_LIB=$(LLVM_DIR)/lib

CC=$(LLVM_BIN)/clang
CXX=$(CC)++
OPT=$(LLVM_BIN)/opt
DIS=$(LLVM_BIN)/llvm-dis
OBJDUMP=$(LLVM_BIN)/llvm-objdump

CXXFLAGS  = -I$(LLVM_DIR)/include
CXXFLAGS += -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -Wno-long-long
CXXFLAGS += -fPIC -fvisibility-inlines-hidden
CXXFLAGS += -fno-exceptions -fno-rtti -std=c++11
CXXFLAGS += -Wall

LDFLAGS = -dynamiclib -Wl,-undefined,dynamic_lookup

target = example
passes = hello dump mutate rtlib fnentry attr srcloc
bin = $(addsuffix .out,$(passes))
optll = $(addsuffix .opt.ll,$(passes))
dis = $(addsuffix .dis.s,$(passes))

all: $(bin) $(dis) $(optll) $(target).ll

%.dylib: %.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.bc: %.cc
	$(CXX) -g -c -emit-llvm -o $@ $<

%.opt.bc %.log: %.dylib $(target).bc
	$(OPT) -load $< -$* $(target).bc > $*.opt.bc 2> $*.log

%.ll: %.bc
	$(DIS) -o=$@ -show-annotations $<

%.out: %.opt.bc hook.o
	$(CXX) -O2 -o $@ $^

%.dis.s: %.out
	$(OBJDUMP) 	-disassemble-all $< > $@

clean:
	$(RM) $$(cat .gitignore)
