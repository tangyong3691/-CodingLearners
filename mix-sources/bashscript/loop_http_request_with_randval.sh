echo "file path: $1"
echo "id: $2"

while [ -f $1 ] 
do
sdata=`date +%s%N`;
dt=${sdata:0-4:4};
echo "dt:${dt}"

curl -v -X POST -d "{\"zzddude\": ${dt}}" http://58.16.86.23:8090/api/v1/$2/telemetry --header "Content-Type:application/json"
sleep 2	

done
