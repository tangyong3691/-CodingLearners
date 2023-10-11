let n=0
while [ $n != 1001 ]
do
let n=n+1
redis-cli -h 192.168.1.129 SET rds2t$n $n
usleep 1000

done
