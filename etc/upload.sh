#!/bin/bash -ex

#
# Publish compiled package to S3 bucket.
#

aws s3 cp clang+llvm*.tar.gz s3://hellollvm/packages/
