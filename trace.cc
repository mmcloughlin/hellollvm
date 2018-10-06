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

    // Construct function name.
    auto str = ConstantDataArray::getString(ctx, F.getName());
    GlobalVariable *global =
        new GlobalVariable(*F.getParent(), str->getType(), true,
                           GlobalValue::LinkageTypes::PrivateLinkage, str);
    global->setAlignment(1);
    auto *fname = ConstantExpr::getPointerCast(global, Type::getInt8PtrTy(ctx));

    // Trace function enter.
    Constant *traceEnter =
        F.getParent()->getOrInsertFunction("trace_enter", functype);
    auto &entry = F.getEntryBlock();
    if (entry.empty()) {
      errs() << "empty\n";
    }
    IRBuilder<> builder(&entry);
    builder.SetInsertPoint(&entry, entry.begin());
    // auto *fname = builder.CreateGlobalStringPtr(F.getName()));
    builder.CreateCall(traceEnter, {fname});

    // Trace before exit instructions.
    Constant *traceLeave =
        F.getParent()->getOrInsertFunction("trace_leave", functype);
    for (auto &B : F) {
      auto *t = B.getTerminator();
      if (isa<ReturnInst>(t) || isa<ResumeInst>(t)) {
        IRBuilder<> builder(t);
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
