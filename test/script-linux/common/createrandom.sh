a=$(($RANDOM))
nalph=abcdefghijkmnpqrstuvwxy
n1=$(($RANDOM % 9 + 1))
n2=$(($RANDOM % 23 + 1))
let n2t=n2+1
n3=$(($RANDOM % 9 + 1))
alp1=${nalph:$n2:1}
echo "${n1}${alp1}${n3}"
