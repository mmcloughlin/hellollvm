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

LDFLAGS = -shared -Wl,-undefined,dynamic_lookup

target = example
hooks = hook
passes = hello dump mutate rtlib fnentry attr srcloc cli returns hookargs trace
bin = $(addsuffix .out,$(passes))
optll = $(addsuffix .opt.ll,$(passes))
dis = $(addsuffix .dis.s,$(passes))
verify = $(addsuffix .v,$(passes))

all: $(bin) $(dis) $(optll) $(verify) $(target).ll $(hooks).ll trace.txt

%.so: %.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.bc: %.cc
	$(CXX) -O1 -g -c -emit-llvm -o $@ $<

%.bc: %.c
	$(CC) -O1 -g -c -emit-llvm -o $@ $<

%.opt.bc %.log: %.so $(target).bc
	$(OPT) -load ./$< -$* $(target).bc > $*.opt.bc 2> $*.log

%.help: %.so
	$(OPT) -load $< -help > $@

%.ll: %.bc
	$(DIS) -o=$@ -show-annotations $<

%.out: %.opt.bc $(hooks).o
	$(CXX) -O2 -o $@ $^

%.v: %.out
	./$< > $@ 2>&1

trace.txt: parsetrace.py trace.v
	python $^ > $@

%.dis.s: %.out
	$(OBJDUMP) 	-disassemble-all $< > $@

tools:
	pip install cxxfilt

clean:
	$(RM) $$(cat .gitignore)
