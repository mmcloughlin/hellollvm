#include "llvm/IR/Attributes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct Attributes : public FunctionPass {
  static char ID;
  Attributes() : FunctionPass(ID) {}

  bool doInitialization(Module &M) override {
    auto annos = M.getNamedGlobal("llvm.global.annotations");
    if (!annos) {
      return false;
    }

    auto a = cast<ConstantArray>(annos->getOperand(0));
    for (int i = 0; i < a->getNumOperands(); i++) {
      auto e = cast<ConstantStruct>(a->getOperand(i));
      if (auto *fn = dyn_cast<Function>(e->getOperand(0)->getOperand(0))) {
        auto anno = cast<ConstantDataArray>(
                        cast<GlobalVariable>(e->getOperand(1)->getOperand(0))
                            ->getOperand(0))
                        ->getAsCString();
        fn->addFnAttr(anno);
      }
    }

    return false;
  }

  bool runOnFunction(Function &F) override {
    if (F.hasFnAttribute("foo")) {
      errs() << F.getName() << " has 'foo'!\n";
    }
    return false;
  }
};
} // namespace

char Attributes::ID = 0;

static RegisterPass<Attributes> X("attr", "List functions with attribute foo");
