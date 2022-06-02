#!/usr/bin/env bash

src_dir=$(pwd)
build_dir="$(pwd)/_build.d"
install_dir="$(pwd)/_install.d"
install_dir="/opt/sw/SuperNEMO-DBD/RED_bridge/install-devel"
RED_bridge_with_tests=false
RED_bridge_with_docs=false

RED_bridge_with_tests=true

if [ -d ${build_dir} ]; then
    rm -fr ${build_dir}
fi

mkdir -p ${build_dir}
cd ${build_dir}

if [ -z ${BX_BAYEUX_INSTALL_DIR} ]; then
    do_snemo_setup
    bayeux_3_5_2_setup
fi

RED_bridge_devel_mode_opt="-DRED_BRIDGE_DEVELOPER_BUILD=OFF -DRED_BRIDGE_GENERATE_DATA=OFF"
RED_bridge_doc_opt="-DRED_BRIDGE_WITH_DOC=OFF"
RED_bridge_tests_opt="-DRED_BRIDGE_ENABLE_TESTING=OFF"

if [ ${RED_bridge_with_tests} = true ]; then
    RED_bridge_tests_opt="-DRED_BRIDGE_ENABLE_TESTING=ON"
fi

if [ ${RED_bridge_with_docs} = true ]; then
    RED_bridge_doc_opt="-DRED_BRIDGE_WITH_DOC=ON"
fi

cmake -DCMAKE_INSTALL_PREFIX=${install_dir} \
      -DBayeux_DIR=$(bxquery --cmakedir) \
      ${RED_bridge_devel_mode_opt} \
      ${RED_bridge_doc_opt} \
      ${RED_bridge_tests_opt} \
      ${rawLtdOpt} \
    ${src_dir}
if [ $? -ne 0 ]; then
    echo >&2 "[error] CMake configuration failed!"
    exit 1
fi

# exit 0

make
if [ $? -ne 0 ]; then
    echo >&2 "[error] Make build failed!"
    exit 1
fi

make install
if [ $? -ne 0 ]; then
    echo >&2 "[error] Make install failed!"
    exit 1
fi

tree ${install_dir}/

exit 0
