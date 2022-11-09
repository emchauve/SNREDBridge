#!/usr/bin/env bash
this_dir=$(pwd)
src_dir="${this_dir}/.."
build_dir="${src_dir}/build.d"
install_dir="${src_dir}/install.d"

if [ -d ${build_dir} ]; then
    rm -fr ${build_dir}
fi

mkdir -p ${build_dir}
cd ${build_dir}

cmake \
    -DCMAKE_INSTALL_PREFIX:PATH="${install_dir}" \
    -GNinja \
    ..


if [ $? -ne 0 ]; then
    echo >&2 "[error] CMake configuration failed!"
    exit 1
fi

# exit 0

ninja
if [ $? -ne 0 ]; then
    echo >&2 "[error] Make build failed!"
    exit 1
fi

ninja install
if [ $? -ne 0 ]; then
    echo >&2 "[error] Make install failed!"
    exit 1
fi

tree ${install_dir}/

exit 0
