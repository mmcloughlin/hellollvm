#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct Returns : public FunctionPass {
  static char ID;
  Returns() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    auto &ctx = F.getContext();
    FunctionType *functype = FunctionType::get(Type::getVoidTy(ctx), false);
    Constant *hook = F.getParent()->getOrInsertFunction("hook", functype);

    for (auto &B : F) {
      for (auto &I : B) {
        auto *op = dyn_cast<ReturnInst>(&I);
        if (!op) {
          continue;
        }

        IRBuilder<> builder(op);
        builder.CreateCall(hook, {});
      }
    }

    return true;
  }
};
} // namespace

char Returns::ID = 0;

static RegisterPass<Returns> X("returns", "Insert hooks before returns");
