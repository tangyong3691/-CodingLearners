echo "check file : zhengdongloopenable"
n=0
echo "argument: when has argu, then only run 1 times"
limitcnt=0
if [ "a$1" != "a" ] ; then
limitcnt=1
fi
exitflag=0
while [ -f  zhengdongloopenable  -a  "$exitflag" != "t" ]
do
let n=(n+1)%60
tdate=`date +"%Y-%m-%dT%T"`
./mosquitto-2.0.14-arm/client/mosquitto_pub -p 8031 -t "PHM/Topics/EigenData/101/0" -m "{\"SampleTime\":\"$tdate.000+0800\",\"EigenType\":0,\"Value\":$n.0}"
sleep 1

if [ $limitcnt -eq 1 ] ; then
exitflag="t"
fi

done
echo "exit"
