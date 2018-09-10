#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct SourceLocation : public FunctionPass {
  static char ID;
  SourceLocation() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    auto *sp = F.getSubprogram();
    if (!sp) {
      errs() << "Missing subprogram, skipping." << '\n';
      return false;
    }

    errs() << sp->getFilename() << ':' << sp->getLine() << '\t' << F.getName()
           << '\n';
    return false;
  }
};
} // namespace

char SourceLocation::ID = 0;

static RegisterPass<SourceLocation>
    X("srcloc", "Print source code location of each function");
