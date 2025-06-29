source /opt/beaglebone_black/environment-setup-armv7at2hf-neon-poky-linux-gnueabi

export TARGET="bbb_can"
export SRC="*.c"
export LDFLAGS="-lpthread -lrt"
export CXXFLAGS="" 
export CPPFLAGS=""

make -j$(cat /proc/cpuinfo | grep -c processor)
cp $TARGET ~/share