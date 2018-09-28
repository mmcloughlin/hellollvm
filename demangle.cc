#include "llvm/Demangle/Demangle.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include <cstdlib>

using namespace llvm;

namespace {

struct Demangle : public FunctionPass {
  static char ID;
  Demangle() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    auto name = F.getName();
    errs().write_escaped(name) << '\n';

    if (!name.startswith("_Z")) {
      errs() << "\tskipping non-mangled name\n";
      return false;
    }

    ItaniumPartialDemangler Demangler;
    assert(!Demangler.partialDemangle(name.data()));

    errs() << "\tisData=" << Demangler.isData() << '\n';
    errs() << "\tisFunction=" << Demangler.isFunction() << '\n';
    errs() << "\tisCtorOrDtor=" << Demangler.isCtorOrDtor() << '\n';
    errs() << "\tisSpecialName=" << Demangler.isSpecialName() << '\n';
    errs() << "\thasFunctionQualifiers=" << Demangler.hasFunctionQualifiers()
           << '\n';

    if (!Demangler.isFunction()) {
      return false;
    }

    size_t size = 1;
    char *buf = static_cast<char *>(std::malloc(size));
    assert(buf);

    buf = Demangler.getFunctionName(buf, &size);
    errs() << "\tFunctionName = " << buf << '\n';

    buf = Demangler.getFunctionDeclContextName(buf, &size);
    errs() << "\tDeclContext = " << buf << '\n';

    buf = Demangler.getFunctionBaseName(buf, &size);
    errs() << "\tBaseName = " << buf << '\n';

    buf = Demangler.getFunctionParameters(buf, &size);
    errs() << "\tParameters = " << buf << '\n';

    buf = Demangler.getFunctionReturnType(buf, &size);
    errs() << "\tReturnType = " << buf << '\n';

    std::free(buf);
    return false;
  }
};

} // namespace

char Demangle::ID = 0;

static RegisterPass<Demangle> X("demangle", "Print demangled function names");
