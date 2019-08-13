echo "file path: $1"
echo "id: $2"

while [ -f $1 ] 
do
sdata=`date +%s%N`;
dt=${sdata:0-2:2};
echo "dt:${dt}"

curl -v -X POST -d "{\"zzddude\": ${dt}}" http://58.16.86.23:8090/api/v1/$2/telemetry --header "Content-Type:application/json" 1>/dev/null 2>&1
if [ "a$3" != "a" ] ; then
	sleep 1
	sdata=`date +%s%N`;
	dt=${sdata:0-2:2};
	curl -v -X POST -d "{\"zzddude\": ${dt}}" http://58.16.86.23:8090/api/v1/$3/telemetry --header "Content-Type:application/json" 1>/dev/null 2>&1
fi
sleep 2	

done
