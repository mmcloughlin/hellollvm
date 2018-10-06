#!/bin/bash -ex

#
# Download and install compiled LLVM package.
#


packagename="clang+llvm-7.0.0"
triple=$(clang++ --version | awk '/Target:/ {print $2}')
fullpackagename="${packagename}-${triple}"
archive="${fullpackagename}.tar.gz"

rm -rf ${fullpackagename}*
aws s3 cp s3://hellollvm/packages/${archive} .
tar xf ${archive}
LLVM_DIR=$(realpath ${fullpackagename})
diff {.,${LLVM_DIR}}/package.sh

echo "export LLVM_DIR=${LLVM_DIR}"
