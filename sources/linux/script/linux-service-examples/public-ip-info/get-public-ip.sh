#!/bin/bash
lpfile="loopfilekeep.txt"
softcmd=$0
softpwd=${softcmd%/*}
lppfile=${softpwd}/$lpfile
mfile="$1"
if  [ "a$mfile" = "a" ] ; then
mfile="/dev/stdout"
fi
mcycle="$2"
if [ "a$mcycle" = "a" ] ; then
mcycle=5
fi
echo ""
echo "info locate: $mfile"
echo "get info cycle: $mcycle"
#file="/home/ty/node_http_server_root/ip.txt"
declare -a mcmd=("curl -4 icanhazip.com" "curl ipinfo.io/ip" "curl ipecho.net/plain")
mpip=""
mdate=""
while [ -f  $lppfile ] 
do
for icmd in "${mcmd[@]}"
do
sleep $mcycle
mpip=`$icmd`
mdate=`date`
echo "" > $mfile
echo "$icmd" >> $mfile
echo $mdate >> $mfile
echo $mpip >> $mfile
curl -k -v -X POST -d "{\"ip\":\"$mpip\",\"date\":\"$mdate\",\"cmd\": \"$icmd\"}" http://cf.beidouapp.com:20038/api/ips  --header "Content-Type:application/json"
done
done
echo "becase loop file not found: $lppfile"
echo "exited"
echo ""
