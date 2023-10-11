let s=1
a=$1
didstart=200
wbstart=2000
touch opc$a.csv
echo "序号,物理对象名称,物理对象类型,内部测点名称,外部测点名称,采样频率(ms)" >> opc$a.csv 
while [ $s -lt 2001 ] 
do
did=$(( (s-1)/10+1+didstart )) 
pid=$(( (s-1)%10+1 ))
wbid=$(( s+wbstart ))
echo "$s,opcppdev$did,device,xtest$pid,wb$wbid,10000" >> opc$a.csv

let s=s+1
done

echo "total:$s" 
