a=$(($RANDOM))
nalph=abcdefghijkmnpqrstuvwxy
nalphan=abcdefghijkmnpqrstuvwxy123456789

tot=0
while [ $tot -lt 80 ] 
do
n1=$(($RANDOM % 9 + 1))
n4=$(($RANDOM % 9 + 1))
n2=$(($RANDOM % 10))
n3=$(($RANDOM % 10))
#nta=$(($RANDOM % 32 ))
#ne=${nalphan:$nta:1}
echo "${n1}${n2}${n3}${n4}"
sleep 0.005
let tot=tot+1
done
