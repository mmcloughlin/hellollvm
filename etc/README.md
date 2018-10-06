Scripts to build LLVM from source with flags favorable for developing passes.

* `CMAKE_BUILD_TYPE=Release` and `LLVM_ENABLE_ASSERTIONS=On` give us a release build but with assertions to catch mistakes in our passes.
* `CMAKE_C_COMPILER` `CMAKE_CXX_COMPILER` set to `clang` and `clang++`. Among other reasons, this will ensure `llvm-config` gives us correct flags for building passes with clang. If built with gcc, we can see warnings like `warning: unknown warning option '-Wno-maybe-uninitialized'; did you mean '-Wno-uninitialized'? [-Wunknown-warning-option]`.
* `LLVM_BUILD_LLVM_DYLIB=On` and `LLVM_LINK_LLVM_DYLIB=On` will ensure that all `libLLVM` symbols are available to dynamically loaded passes.
* `LLVM_BINUTILS_INCDIR=${BINUTILS_INCDIR}` will build the gold linker plugin, if `BINUTILS_INCDIR` is set appropriately.

In this directory:

* [`package.sh`](package.sh) builds LLVM and related packages in a temporary working directory and writes an archive to the current directory.
* [`upload.sh`](upload.sh) uploads the built archive to an S3 bucket for use by [Travis CI](https://travis-ci.org/mmcloughlin/hellollvm).