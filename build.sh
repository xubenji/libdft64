# 在使用pin之前运行此文件
# 这个文件是搭建环境用的
# 例如：
# bash build.sh libdft-dta
# 编译libdft-dta

PWD=`pwd`

# 两个if判断是否已经安装了pin
if [ ! -f "$PWD/pin.info" ];then
	echo "pin file here"
	echo "" > $PWD/pin.info
fi

pininfo=`cat pin.info | grep PIN_ROOT | wc -l`
if [  $pininfo == 0  ]; then
	echo $pininfo
	echo "install pin...."
	bash install_pin.sh > pin.info
else	
	echo "pin exist!"
fi


PIN_ROOT=`awk '{print}' ./pin.info |tail -n 1`
echo $PIN_ROOT > ./pin.info
MAKE=`awk '{gsub("export","make",$1);print}' ./pin.info`

$MAKE

if [ $1 ]; then
	sed -i '31c TOOL_ROOTS := '$1'' $PWD/tools/makefile.rules
	$MAKE
fi


		

