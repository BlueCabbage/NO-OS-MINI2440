#!/bin/sh

src_dir='/home/zhaochao/Desktop/hardware/hardware/lcd/lib/'
des_dir=${PWD}

echo 'Src_dir:' ${src_dir}
echo 'Des_dif:' ${des_dir}

echo '##############################################3'

src_files=''

cd ${src_dir}
echo 'PWM' ${PWD}

#for file in $(find ./ -type f)
for file in $(ls ./)
do
	if [ -f "${file}" ]
	then
		dir=`dirname $file`
		filename=`basename $file`
		echo 'Doing-Src_files:' ${dir}/${file}
		cat ${filename} > ${des_dir}/${filename}
	fi
done

cd ${des_dir}
echo '###################################################'
echo 'done.'
	

