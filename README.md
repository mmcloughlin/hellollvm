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

## Q&A

**Why does the `RegisterPass<>` method not work when loaded into `clang`?** Under this method the pass dynamic library is loaded and enabled with `opt -load <pass lib> -<pass name>`. By default, the `clang` CLI emulates `gcc` as much as possible. In particular it does not directly expose LLVM optimization passes. To get it to work directly with `clang` we need to [register the pass in code](http://www.cs.cornell.edu/~asampson/blog/clangpass.html) using the `PassManagerBuilder`.

**How do you detect function attributes like `__attribute__((annotate("foo")))`?** These are recorded in a global variable called `llvm.global.annotations`. Brandon Holt demonstrates a technique to [parse this and attach them as function attributes](http://bholt.org/posts/llvm-quick-tricks.html).

**What is the return value of `doInitialization` and `runOnFunction`?** Return `true` if any changes were made.

**How do you determine original source code location of an LLVM Function?** This requires the program is compiled with debug information `-g`. The information can be accessed through [`getSubprogram`](http://llvm.org/doxygen/classllvm_1_1Function.html#a4d834f9897d15e3a6349063b5d637cd8) method.

**How do you define a string constant?** Use `CreateGlobalStringPtr(...)` on an `IRBuilder`.

**How do you create a floating-point constant?** Use [`ConstantFP`](http://llvm.org/doxygen/classllvm_1_1ConstantFP.html), for example `ConstantFP::get(Type::getDoubleTy(ctx), 42.0)`.

## Todo

- [x] Pass arguments to hook functions
- [ ] Function exit in the case of exceptions
- [ ] Function exit with tail calls?
- [ ] How to inline hook function calls?
- [ ] Combine to a full tracer pass
- [ ] Call c++ hook function (handle mangling)
- [ ] Get pass working directly with clang (not via `opt`)