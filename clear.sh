#!/bin/sh

CUR_DIR_PATH=$(cd "$(dirname "$0")" && pwd)

#for FILENAME in $(find . -type f -name remove.sh -print | sed 's/^\.\///')
for FILE_PATH in $(find . -type f -name remove.sh)
do
    DIR_PATH="$(dirname $FILE_PATH)"
    FILE_NAME="$(basename $FILE_PATH)"
    echo $DIR_PATH
    cd $DIR_PATH
	./$FILE_NAME
	cd $CUR_DIR_PATH
done

rm -rf CMakeCache.txt CMakeFiles Makefile cmake_install.cmake install_manifest.txt