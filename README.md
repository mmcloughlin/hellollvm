# hellollvm

Basic "Hello World" LLVM passes.

Based on [LLVM for Grad
Students](https://www.cs.cornell.edu/~asampson/blog/llvm.html) and [skeleton
code](https://github.com/sampsyo/llvm-pass-skeleton).

## Passes

* [`hello`](hello.cc) prints out the name of every function
* [`dump`](dump.cc) dumps LLVM IR for the first function, basic block and instruction it sees
* [`mutate`](mutate.cc) modifies the first binary operator of each function to a multiplication
* [`rtlib`](rtlib.cc) inserts a call to a _hook_ function after the first binary operator
* [`fnentry`](fnentry.cc) inserts call at function entry
* [`attr`](attr.cc) finds functions with attribute `foo`
* [`srcloc`](srcloc.cc) prints `file:line` metadata from each function (requires debug information)
* [`cli`](cli.cc) shows how to define a command-line option
* [`returns`](returns.cc) inserts calls before function returns
* [`hookargs`](hookargs.cc) calls a hook function with arguments
* [`trace`](trace.cc) inserts hooks at function enter and exit points ([script to verify correctness](parsetrace.py))
* [`demangle`](demangle.cc) is `hello` with C++ name demangling

## Q&A

**Why does the `RegisterPass<>` method not work when loaded into `clang`?** Under this method the pass dynamic library is loaded and enabled with `opt -load <pass lib> -<pass name>`. By default, the `clang` CLI emulates `gcc` as much as possible. In particular it does not directly expose LLVM optimization passes. To get it to work directly with `clang` we need to [register the pass in code](http://www.cs.cornell.edu/~asampson/blog/clangpass.html) using the `PassManagerBuilder`.

**How do you detect function attributes like `__attribute__((annotate("foo")))`?** These are recorded in a global variable called `llvm.global.annotations`. Brandon Holt demonstrates a technique to [parse this and attach them as function attributes](http://bholt.org/posts/llvm-quick-tricks.html).

**What is the return value of `doInitialization` and `runOnFunction`?** Return `true` if any changes were made.

**How do you determine original source code location of an LLVM Function?** This requires the program is compiled with debug information `-g`. The information can be accessed through [`getSubprogram`](http://llvm.org/doxygen/classllvm_1_1Function.html#a4d834f9897d15e3a6349063b5d637cd8) method.

**How do you define a string constant?** Use `CreateGlobalStringPtr(...)` on an `IRBuilder`. Unfortunately I encountered an [obscure segfault with LLVM 7.0.0 on Ubuntu](https://stackoverflow.com/questions/48577492/llvm-irbuildercreateglobalstringptr-segmentation-fault). A more manual approach which avoided this problem was to use `ConstantDataArray::getString`, followed by `new GlobalVariable` and `ConstantExpr::getPointerCast`.

**How do you create a floating-point constant?** Use [`ConstantFP`](http://llvm.org/doxygen/classllvm_1_1ConstantFP.html), for example `ConstantFP::get(Type::getDoubleTy(ctx), 42.0)`.

**Which instructions exit a function?** [Terminator instructions](https://llvm.org/docs/LangRef.html#terminator-instructions) end basic blocks, either to exit the function or to branch to another part of the same function. The terminator instructions are:

* `ret` returns control flow back to the caller.
* `br` transfers control flow to one of two other blocks in the current function.
* `switch` is a generalization of `br`, allowing control flow to several other blocks based on a value
* `indirectbr` jumps to another location by addressing into a list of labels
* `invoke` is a call instruction that handles alternative control flow in the case of an exception
* `resume` propagates an exception
* `catchswitch` switches between exception catch handlers
* `catchret`
* `cleanupret`
* `unreachable` informs the optimizer that a piece of code us unreachable

Use `getTerminator` method of [`BasicBlock`](http://llvm.org/doxygen/classllvm_1_1BasicBlock.html) to find.

**How do you demangle C++ names?** LLVM provides its own implementation of `__cxa_demangle` (from `cxxabi.h`) in [`Demangle.h`](http://llvm.org/doxygen/Demangle_8h.html), specifically `itaniumDemangle`. The [symbolizer](https://llvm.org/docs/CommandGuide/llvm-symbolizer.html) source code provides a useful [example](https://github.com/llvm-mirror/llvm/blob/0444f576689f40fd292478aff54a0f6a9ceebfdb/lib/DebugInfo/Symbolize/Symbolize.cpp#L474-L482). The [`ItaniumPartialDemangler`](http://llvm.org/doxygen/structllvm_1_1ItaniumPartialDemangler.html) (introduced in LLVM 7) enables access to subcomponents of the mangled name.

## Todo

- [x] Pass arguments to hook functions
- [ ] Function exit in the case of exceptions
- [ ] Function exit with tail calls?
- [ ] How to inline hook function calls?
- [ ] Combine to a full tracer pass
- [ ] Call c++ hook function (handle mangling)
- [ ] Get pass working directly with clang (not via `opt`)
- [ ] [New style passes](https://medium.com/@mshockwave/writing-llvm-pass-in-2018-part-i-531c700e85eb)?