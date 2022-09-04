# PWD=`pwd`
# PIN_ROOT=`awk '{print}' '$PWD'/pin.info |tail -n 1`
# MAKE=`awk '{gsub("export"," ",$1);print}' ./pin.info`
# echo $MAKE > ./pin.info
# MAKE=`awk '{gsub("PIN_ROOT="," ",$1);print}' ./pin.info`
/root/pin-3.20-98437-gf02b61307-gcc-linux/pin -t ./tools/obj-intel64/track.so -- ./tools/obj-intel64/mini_test.exe  ./tools/cur_input
