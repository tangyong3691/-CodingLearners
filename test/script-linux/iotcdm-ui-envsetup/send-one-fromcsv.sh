file="temp/test1.csv"
while read line; do
##echo -e "$line\n" > /home/docker_data/volumes/iotboard_iot_nodered_data/_data/ttcsv1tmp.csv
echo "$line" 
curl -X POST http://192.168.1.225:51880/posttestit -H "Content-Type:application/json" -d "{\"data\":\"$line\"}"
sleep 3
done < $file
echo "exit"

