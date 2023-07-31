#!/bin/sh
#
# depend modpoll:  https://www.modbusdriver.com/modpoll.html
offtm=`timedatectl | grep 'Time zone' | grep -oP '\d+'`
offtm=`echo $offtm | sed 's/^0*//'`
if [ "a$offtm" == "a" ] ; then
offtm=0
else
offtm=$((  (offtm - offtm % 100) / 100 ))
fi
#/usr/bin/redis-cli set allzerovals  "[0,0,0,0,0]"
#/usr/bin/redis-cli set zeroval "0"
wordval=65535
while true ; do
tmss=`date +%s`
curs=$(( tmss%60 ))
curm=$(( ( (tmss-curs) /60 ) % 60 ))
curh=$(( ((tmss-curs-curm*60)/3600)%24 + offtm ))
tmdata=$(( curh*10000+curm*100+curs ))
/home/tangyong/test/xc-gzcl-wlwgl-assist-env/modpoll/x86_64-linux-gnu/modpoll -m tcp -r 10000 -c 1 -0  192.168.1.231 $wordval
/home/tangyong/test/xc-gzcl-wlwgl-assist-env/modpoll/x86_64-linux-gnu/modpoll -m tcp -r 10001 -c 1 -0  192.168.1.231 $curh
/home/tangyong/test/xc-gzcl-wlwgl-assist-env/modpoll/x86_64-linux-gnu/modpoll -m tcp -r 10002 -c 1 -0  192.168.1.231 $curm
/home/tangyong/test/xc-gzcl-wlwgl-assist-env/modpoll/x86_64-linux-gnu/modpoll -m tcp -r 10003 -c 1 -0  192.168.1.231 $curs

if [ $wordval -eq 0 ] ; then
wordval=65535
else
wordval=$(( wordval - 1 ))
fi

sleep 1

done

