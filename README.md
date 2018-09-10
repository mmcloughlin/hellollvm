# hellollvm

Basic "Hello World" LLVM passes.

Based on [LLVM for Grad
Students](https://www.cs.cornell.edu/~asampson/blog/llvm.html) and [skeleton
code](https://github.com/sampsyo/llvm-pass-skeleton).

## Q&A

**Why does the `RegisterPass<>` method not work when loaded into `clang`?** Under this method the pass dynamic library is loaded and enabled with `opt -load <pass lib> -<pass name>`. By default, the `clang` CLI emulates `gcc` as much as possible. In particular it does not directly expose LLVM optimization passes. To get it to work directly with `clang` we need to [register the pass in code](http://www.cs.cornell.edu/~asampson/blog/clangpass.html) using the `PassManagerBuilder`.
