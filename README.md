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
* [`fnentry`](fnentry.cc) inserts call to _hook_ at function entry

## Q&A

**Why does the `RegisterPass<>` method not work when loaded into `clang`?** Under this method the pass dynamic library is loaded and enabled with `opt -load <pass lib> -<pass name>`. By default, the `clang` CLI emulates `gcc` as much as possible. In particular it does not directly expose LLVM optimization passes. To get it to work directly with `clang` we need to [register the pass in code](http://www.cs.cornell.edu/~asampson/blog/clangpass.html) using the `PassManagerBuilder`.

**How do you detect function attributes like `__attribute__((annotate("foo"))`?*** These are recorded in a global variable called `llvm.global.annotations`. Brandon Holt demonstrates a technique to [parse this and attach them as function attributes](http://bholt.org/posts/llvm-quick-tricks.html).
