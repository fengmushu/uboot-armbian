#!/bin/bash

build_target()
{
    # ENV
    which arm-linux-gnueabihf-gcc 2>/dev/null || {
        echo "需要设置环境变量, 指定交叉编译工具链: arm-linux-gnueabihf- 的路径, 例如:"
        echo 'export PATH=$PATH:/xxx/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabihf/bin'
        exit -1
    }
    # BUILD
    make CROSS_COMPILE=arm-linux-gnueabihf- || exit -1
}

patch_src()
{
    git diff sun8i-patched > z-1000-QPT-uboot.patch
}

usage()
{
    echo "./make.sh <build> --- 编译uboot"
    echo "          <patch> --- 生成armbian补丁, 需要手动拷贝到: userpatches/u-boot/u-boot-sunxi/branch_default/"
    echo ""
    echo "  注意: 若修改.config默认选项, 需要同步更新armbian/config相关配置"
}

case $1 in
    patch)
        echo "Patch code modify from armbian-original."
        patch_src
    ;;
    build)
        echo "Build u-boot-sunxi-with-spl.bin"
        build_target
    ;;
    *)
        usage
    ;;
esac