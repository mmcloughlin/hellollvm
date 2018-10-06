#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct HookArgs : public FunctionPass {
  static char ID;
  HookArgs() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    auto &ctx = F.getContext();

    FunctionType *functype = FunctionType::get(
        Type::getVoidTy(ctx), {Type::getInt8PtrTy(ctx), Type::getDoubleTy(ctx)},
        false);
    Constant *hook = F.getParent()->getOrInsertFunction("metric", functype);

    auto str = ConstantDataArray::getString(ctx, F.getName());
    GlobalVariable *global =
        new GlobalVariable(*F.getParent(), str->getType(), true,
                           GlobalValue::LinkageTypes::PrivateLinkage, str);
    global->setAlignment(1);
    auto *fname = ConstantExpr::getPointerCast(global, Type::getInt8PtrTy(ctx));

    auto &entry = F.getEntryBlock();
    IRBuilder<> builder(&entry);
    builder.SetInsertPoint(&entry, entry.begin());
    builder.CreateCall(hook,
                       {fname, ConstantFP::get(Type::getDoubleTy(ctx), 42.0)});

    return true;
  }
};
} // namespace

char HookArgs::ID = 0;

static RegisterPass<HookArgs> X("hookargs",
                                "Call a hook function with arguments");
