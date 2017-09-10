#!/bin/bash
# run8core.sh --- 
# 
# Description: 
# Author: Hongyi Wu(吴鸿毅)
# Email: wuhongyi@qq.com 
# Created: 日 5月  7 10:46:54 2017 (+0800)
# Last-Updated: 日 5月  7 22:39:02 2017 (+0800)
#           By: Hongyi Wu(吴鸿毅)
#     Update #: 26
# URL: http://wuhongyi.cn 

cpu=8
totalrun=24

i=0

while [[ "$i" -lt "$totalrun" ]]
do
# echo `ps -e | grep -c "wu"`
if [[ `ps -e | grep -c "wu"` -lt "$cpu" ]]
then
    ((i++))
    echo "Run Number: $i"
    ./wu > /dev/null  &
    # sleep 20
fi    

# sleep 10    
    
done

# 
# run8core.sh ends here
