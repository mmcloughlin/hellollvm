#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include <string>

using namespace llvm;

static cl::opt<std::string> Who("who", cl::desc("Who to greet"),
                                cl::value_desc("name"), cl::init("LLVM"));

namespace {
struct CLIExample : public FunctionPass {
  static char ID;
  CLIExample() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    errs() << F.getName() << " says 'Hello, " << Who << "'!\n";
    return false;
  }
};
} // namespace

char CLIExample::ID = 0;

static RegisterPass<CLIExample> X("cli", "Example command-line option");
