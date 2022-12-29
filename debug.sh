MAIN_PATH=$(pwd)

PIN_ROOT=`awk '{print}' $MAIN_PATH/pin.info |tail -n 1`
echo $PIN_ROOT > ./pin.info
MAKE=`awk '{gsub("export"," ",$1);print}' ./pin.info`

MAKE_COMMAND="make DEBUG=1"$MAKE
$MAKE_COMMAND
if [ $1 ]; then
	sed -i '31c TOOL_ROOTS := '$1'' $PWD/tools/makefile.rules
	$MAKE_COMMAND
fi



rm $MAIN_PATH/pin.log
rm $MAIN_PATH/pid.txt
#不同的pintool这里会有很大差别
`$MAIN_PATH/pin-3.20-98437-gf02b61307-gcc-linux/pin -pause_tool 30 -t $MAIN_PATH/tools/obj-intel64/$1.so -- $MAIN_PATH/tools/obj-intel64/mini_test.exe  $MAIN_PATH/tools/cur_input  | tee pin.log` &
sleep 1

grep 'set sysroot /not/existing/dir' pin.log > $MAIN_PATH/.gdbinit
grep -oE 'pid [0-9]+' pin.log | grep -oE '[0-9]+' > $MAIN_PATH/pid.txt
awk '/add-symbol-file/ {print $0}' pin.log >> $MAIN_PATH/.gdbinit
cat initcmd >> .gdbinit

value=$(cat $MAIN_PATH/pid.txt)
sed -i "s/\"pid\": \".*\",/\"pid\": \"$value\",/g" $MAIN_PATH/.vscode/launch.json

ls
gdb attach $value
#gdbserver 127.0.0.1:1234  --attach $value




