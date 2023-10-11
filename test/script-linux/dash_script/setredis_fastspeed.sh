let num=1
let startid=50
while [ $num -lt 501 ] ; do
id=$(( num + startid ))
/usr/bin/redis-cli -p 16379 set fastc1 "$num"
num=$(( num + 1 ))
sleep 0.02
done


