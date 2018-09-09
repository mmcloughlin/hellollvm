#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct Dumper : public FunctionPass {
  static char ID;
  Dumper() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    errs() << "Function body:\n";
    F.print(llvm::errs());

    for (auto &B : F) {
      errs() << "Basic block:\n";
      B.print(llvm::errs());

      for (auto &I : B) {
        errs() << "Instruction: ";
        I.print(llvm::errs());
      }
    }
    return false;
  }
};
} // namespace

char Dumper::ID = 0;

static RegisterPass<Dumper> X("dump", "Dump function code",
                              false /* Only looks at CFG */,
                              false /* Analysis Pass */);
