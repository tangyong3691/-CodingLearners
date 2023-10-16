let s=1
a=$1
loopst=$2
didstart=30
wbstart=300
if [ "a$loopst" == "a" ] ; then
loopst=1000
fi
loopst=$(( loopst+1 ))
touch rds$a.csv
echo "序号,物理对象名称,物理对象类型,内部测点名称,redis key,下标,采样周期（ms，不填写默认5000）" >> rds$a.csv 
while [ $s -lt $loopst ] 
do
did=$(( (s-1)/10+1+didstart )) 
pid=$(( (s-1)%10+1 ))
wbid=$(( s+wbstart ))
echo "$s,rdsppdev$did,device,xtest$pid,zzrd$wbid,0,1000" >> rds$a.csv

let s=s+1
done

echo "total:$s" 
