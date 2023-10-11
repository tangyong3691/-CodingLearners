#!/bin/sh
offtm=`timedatectl | grep 'Time zone' | grep -oP '\d+'`
offtm=`echo $offtm | sed 's/^0*//'`
if [ "a$offtm" == "a" ] ; then
offtm=0
else
offtm=$((  (offtm - offtm % 100) / 100 ))
fi
/usr/bin/redis-cli set allzerovals  "[0,0,0,0,0]"
/usr/bin/redis-cli set zeroval "0"
while true ; do
tmss=`date +%s`
curs=$(( tmss%60 ))
curm=$(( ( (tmss-curs) /60 ) % 60 ))
curh=$(( ((tmss-curs-curm*60)/3600)%24 + offtm ))
tmdata=$(( curh*10000+curm*100+curs ))
/usr/bin/redis-cli set tmdata "$tmdata"
/usr/bin/redis-cli set tmhms  "[$curh,$curm,$curs]"
sleep 1

done
