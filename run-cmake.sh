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

echo "Building for $buildtype"

if [ -z $2 ]
then
  compiler=gcc
else
  compiler=$2
fi

echo "Using compiler $compiler"


builddir=build-$buildtype

if [ ! -d $builddir ]
then
  mkdir $builddir
fi

# change into debug dir
cd $builddir

pwd

cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=${buildtype^} -DCMAKE_C_COMPILER=$compiler ..

cd ..
