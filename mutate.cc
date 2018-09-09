#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct Mutate : public FunctionPass {
  static char ID;
  Mutate() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    errs() << "Function " << F.getName() << '\n';
    for (auto &B : F) {
      for (auto &I : B) {
        auto *op = dyn_cast<BinaryOperator>(&I);
        if (!op) {
          continue;
        }

        errs() << "Found binary operator.\n";
        I.print(llvm::errs());

        // Build multiplication instruction with the same operands.
        IRBuilder<> builder(op);
        Value *lhs = op->getOperand(0);
        Value *rhs = op->getOperand(1);
        Value *mul = builder.CreateMul(lhs, rhs);

        // Replaces uses of the result with the multiplication.
        for (auto &U : op->uses()) {
          U.getUser()->setOperand(U.getOperandNo(), mul);
        }

        return true;
      }
    }
    return false;
  }
};
} // namespace

char Mutate::ID = 0;

static RegisterPass<Mutate> X("mutate", "Swap binary op for multiply");
