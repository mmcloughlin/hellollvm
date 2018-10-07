#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/Regex.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

static cl::opt<std::string>
    Pattern("regex",
            cl::desc("Print functions matching this regular expression"),
            cl::init("add"));

namespace {
struct Regexp : public FunctionPass {
  static char ID;
  Regexp() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    Regex R(Pattern);
    if (R.match(F.getName())) {
      errs() << "Match: ";
      errs().write_escaped(F.getName()) << '\n';
    }
    return false;
  }
};
} // namespace

char Regexp::ID = 0;

static RegisterPass<Regexp> X("regexp",
                              "Print functions matching a regular expression");
