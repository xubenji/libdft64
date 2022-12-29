set sysroot /not/existing/dir
add-symbol-file /root/libdft/libdft64/tools/obj-intel64/track.so 0x7f84ae8cc1a0 -s .data 0x7f84aeae88c0 -s .bss 0x7f84aeaeff40

b track.cpp:32
layout src
set disassembly-flavor intel 

