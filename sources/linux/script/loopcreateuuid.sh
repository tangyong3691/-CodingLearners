let s=1
a=$1
touch uuidlist${a}.txt
while [ $s -lt 20000 ] 
do
cat /proc/sys/kernel/random/uuid >> uuidlist${a}.txt
sleep 0.001
let s=s+1
done

echo $s 
