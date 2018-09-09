#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct RTLib : public FunctionPass {
  static char ID;
  RTLib() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    auto &ctx = F.getContext();
    FunctionType *functype = FunctionType::get(Type::getVoidTy(ctx), false);
    Constant *hook = F.getParent()->getOrInsertFunction("hook", functype);

    errs() << "Function " << F.getName() << '\n';
    for (auto &B : F) {
      for (auto &I : B) {
        auto *op = dyn_cast<BinaryOperator>(&I);
        if (!op) {
          continue;
        }

        IRBuilder<> builder(op);
        builder.SetInsertPoint(&B, ++builder.GetInsertPoint());
        builder.CreateCall(hook, {});

        return true;
      }
    }
    return false;
  }
};
} // namespace

char RTLib::ID = 0;

static RegisterPass<RTLib> X("rtlib", "Insert call to runtime library");
