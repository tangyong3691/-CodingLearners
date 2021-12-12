#echo "get public ip"
#echo "depend command: dig +short myip.opendns.com @resolver1.opendns.com"

if [ "a$1" == "a" ] ; then
ipfile="/dataext/soft/myip_infos"
else
ipfile=$1
fi

echo "$ipfile"

if ! [ -e $ipfile ] ; then
echo "create $ipfile"
touch $ipfile
fi

echo "check loop enable file: /dataext/soft/loopok"
while [ -f /dataext/soft/loopok ] 
do
currentip=`dig +short myip.opendns.com @resolver1.opendns.com`

if [ "a$currentip" == "a" ] ; then
thisdate=`date`
echo "get ip error , $thisdate"
fi

chkit=`cat $ipfile | grep $currentip`

if [ "a$chkit" == "a" ] ; then
echo "add ip: $currentip"
ipadddate=`date`
echo "$currentip   $ipadddate" >> $ipfile
fi

sleep 3600
done

