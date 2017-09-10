#!/bin/bash

# autorun.sh --- 
# 
# Description: 
# Author: Hongyi Wu(吴鸿毅)
# Email: wuhongyi@qq.com 
# Created: 二 2月 21 23:15:14 2017 (+0800)
# Last-Updated: 四 3月 23 14:37:30 2017 (+0800)
#           By: Hongyi Wu(吴鸿毅)
#     Update #: 3
# URL: http://wuhongyi.cn

#修改这里的 i<=10 来控制循环次数
for (( i=1; i<=10; i++ ))
do
    echo "Run Number: $i"
    # ./wu > /dev/null
    ./wu
    # sleep 3
done

# 
# autorun.sh ends here
