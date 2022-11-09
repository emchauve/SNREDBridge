#!/usr/bin/env bash
this_dir=$(pwd)
src_dir="${this_dir}/.."
build_dir="${src_dir}/build.d"
install_dir="${src_dir}/install.d"

rm -fr ${build_dir} ${install_dir}
