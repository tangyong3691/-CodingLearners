
echo "loopfile: rateinfo_enable_loop"
while [ -f rateinfo_enable_loop ] 
do
rateinfo=`tail -n 200 ./iot/iot-node/log/beidouapp.log | grep CassandraBufferedRateExecutor | tail -n 1`
if [ "a$rateinfo" != "a" ] ; then
echo $rateinfo
echo ""
fi
sleep 10
done
echo "rateinfo loop exited"
echo ""
