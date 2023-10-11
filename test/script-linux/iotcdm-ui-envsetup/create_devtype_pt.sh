let s=1
a=$1
didstart=0
wbstart=1001
touch xcdevtypept$a.csv
echo "序号,设备类型,测点名称,数据类型,单位,说明" >> xcdevtypept$a.csv 
while [ $s -lt 501 ] 
do
ptn=$(( (s-1)%50+1+didstart )) 
tyn1=$(( (s-1)/50 ))
tyn2=$(( tyn1/2 + 1 ))
tyn3=$(( tyn1%2 + 1 ))
echo "$s,zy$tyn2-类型$tyn3,xtest$ptn,double,," >> xcdevtypept$a.csv

let s=s+1
done

echo "total:$s" 
