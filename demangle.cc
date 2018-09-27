#include "llvm/Demangle/Demangle.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

std::string DemangleName(StringRef name) {
  if (!name.startswith("_Z")) {
    return name;
  }

  int status = 0;
  char *demangled = itaniumDemangle(name.data(), nullptr, nullptr, &status);
  if (status != 0) {
    return name;
  }
  std::string result = demangled;
  free(demangled);
  return result;
}

struct Demangle : public FunctionPass {
  static char ID;
  Demangle() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    auto name = DemangleName(F.getName());
    errs() << "Hello: ";
    errs().write_escaped(name) << '\n';
    return false;
  }
};

} // namespace

char Demangle::ID = 0;

static RegisterPass<Demangle> X("demangle", "Print demangled function names");
