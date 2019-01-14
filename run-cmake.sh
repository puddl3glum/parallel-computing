#!/usr/bin/env bash

if [[ $1 == "debug" ]]
then
  buildtype="debug"
elif [[ $1 == "release" ]]
then
  buildtype="release"
else
  echo "Invalid build type. Expected debug or release"
  exit
fi

builddir=build-$buildtype

if [ ! -d $builddir ]
then
  mkdir $builddir
fi

# change into debug dir
cd $builddir

pwd

cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=${buildtype^} -DCMAKE_C_COMPILER=/bin/clang ..

cd ..
