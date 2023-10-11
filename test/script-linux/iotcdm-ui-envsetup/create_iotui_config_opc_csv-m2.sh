let s=1
a=$1
didstart=0
wbstart=1000
touch opc$a.csv
echo "序号,物理对象名称,物理对象类型,内部测点名称,外部测点名称,采样频率(ms)" >> opc$a.csv 
while [ $s -lt 5001 ] 
do
dn1=$(( (s-1) / 50 ))
dn2=$(( dn1%10+1 )) 
dn3=$(( dn1/10 ))
dn4=$(( dn3%2 + 1 ))
dn5=$(( dn3/2 + 1 ))
did=$(( (s-1)/200+1+didstart )) 
pid=$(( (s-1)%50+1 ))
wbid=$(( s+wbstart ))
echo "$s,zy$dn5-类型$dn4-设备$dn2,device,xtest$pid,g1a$wbid,1000" >> opc$a.csv

let s=s+1
done

echo "total:$s" 
