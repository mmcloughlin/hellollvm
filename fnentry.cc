#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct FnEntry : public FunctionPass {
  static char ID;
  FnEntry() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    auto &ctx = F.getContext();
    FunctionType *functype = FunctionType::get(Type::getVoidTy(ctx), false);
    Constant *hook = F.getParent()->getOrInsertFunction("hook", functype);

    auto &entry = F.getEntryBlock();
    entry.print(errs());
    IRBuilder<> builder(&entry);
    builder.SetInsertPoint(&entry, entry.begin());
    builder.CreateCall(hook, {});

    return true;
  }
};
} // namespace

char FnEntry::ID = 0;

static RegisterPass<FnEntry> X("fnentry", "Insert hook at function entry");
