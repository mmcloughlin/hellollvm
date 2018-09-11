#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct Trace : public FunctionPass {
  static char ID;
  Trace() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    auto &ctx = F.getContext();
    FunctionType *functype = FunctionType::get(
        Type::getVoidTy(ctx), {Type::getInt8PtrTy(ctx)}, false);

    // Trace function enter.
    Constant *traceEnter =
        F.getParent()->getOrInsertFunction("trace_enter", functype);
    auto &entry = F.getEntryBlock();
    entry.print(errs());
    IRBuilder<> builder(&entry);
    builder.SetInsertPoint(&entry, entry.begin());
    auto *fname = builder.CreateGlobalStringPtr(F.getName());
    builder.CreateCall(traceEnter, {fname});

    // Trace before return instructions.
    Constant *traceLeave =
        F.getParent()->getOrInsertFunction("trace_leave", functype);
    for (auto &B : F) {
      for (auto &I : B) {
        auto *op = dyn_cast<ReturnInst>(&I);
        if (!op) {
          continue;
        }

        IRBuilder<> builder(op);
        builder.CreateCall(traceLeave, {fname});
      }
    }

    return true;
  }
};
} // namespace

char Trace::ID = 0;

static RegisterPass<Trace> X("trace",
                             "Insert hooks at function enter and exit");
