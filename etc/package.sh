#!/bin/bash -ex

name=clang+llvm
version=7.0.0
j=8

#-----------------------------------------------------------------------------

packagename=${name}-${version}

#-----------------------------------------------------------------------------

cwd=$(pwd -P)
self=$(realpath "${BASH_SOURCE[0]}")
selfbase=$(basename ${self})

workdir=$(mktemp -d)
cd ${workdir}

#-----------------------------------------------------------------------------

packagedir=${workdir}/pkg
installdir=${packagedir}/${packagename}
builddir=${workdir}/build

mkdir -p ${builddir}
mkdir -p ${installdir}

#-----------------------------------------------------------------------------

cmake_archive_name=cmake-3.4.3
cmake_archive=${cmake_archive_name}.tar.gz
wget http://www.cmake.org/files/v3.4/${cmake_archive}
tar -xvzf ${cmake_archive}
cd ${cmake_archive_name}
./configure
make -j${j}
CMAKE=${workdir}/${cmake_archive_name}/bin/cmake
cd ${workdir}

#-----------------------------------------------------------------------------

llvm_archive_name=llvm-${version}.src
llvm_archive=${llvm_archive_name}.tar.xz
wget http://releases.llvm.org/${version}/${llvm_archive}

tar -xf ${llvm_archive}
LLVM_SRC_ROOT=${workdir}/${llvm_archive_name}

#-----------------------------------------------------------------------------

download_package()
{
    archive_name=$1-${version}.src
    archive=${archive_name}.tar.xz
    wget http://releases.llvm.org/${version}/${archive}

    tar -xf ${archive}
    mv ${archive_name} ${LLVM_SRC_ROOT}/$2
}

download_package cfe tools/clang
download_package clang-tools-extra tools/clang/tools/extra
download_package lld tools/lld
#download_package lldb tools/lldb
download_package polly tools/polly
download_package compiler-rt projects/compiler-rt
download_package openmp projects/openmp
download_package libcxx projects/libcxx
download_package libcxxabi projects/libcxxabi
download_package test-suite projects/test-suite
download_package libunwind projects/libunwind

#-----------------------------------------------------------------------------

cd ${builddir}
${CMAKE} -G 'Unix Makefiles' \
    -DCMAKE_INSTALL_PREFIX=${installdir} \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_ENABLE_ASSERTIONS=On \
    -DLLVM_BUILD_LLVM_DYLIB=On \
    -DLLVM_LINK_LLVM_DYLIB=On \
    -DLLVM_BINUTILS_INCDIR=${BINUTILS_INCDIR} \
    ${LLVM_SRC_ROOT}
make -j${j} install
cd ${workdir}

#-----------------------------------------------------------------------------

cp ${self} ${installdir}
cat > ${installdir}/README.txt <<EOF
See ${selfbase} for the exact build commands.
EOF

#-----------------------------------------------------------------------------

cd ${packagedir}
archive=${packagename}.tar.gz
tar czf ${archive} ${packagename}
cp ${archive} ${cwd}
cd ${workdir}

#-----------------------------------------------------------------------------

ls -l ${workdir}
