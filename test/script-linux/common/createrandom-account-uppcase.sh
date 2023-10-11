a=$(($RANDOM))
nalph=ABCDEFGHIJKMNPQRSTUVWXY
nalphan=ABCDEFGHIJKMNPQRSTUVWXY123456789
n1=$(($RANDOM % 9 + 1))
n2=$(($RANDOM % 23 ))
nta=$(($RANDOM % 32 ))
let n2t=n2+1
ne=${nalphan:$nta:1}
alp1=${nalph:$n2:1}
echo "bdhUSER&${n1}${alp1}${ne}!"
