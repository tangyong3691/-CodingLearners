let s=1
a=$1
touch devadd${a}.sql
while [ $s -lt 1000 ] 
do
did=$(( s + 10000 ))
echo "INSERT INTO public.device(id, name, device_type_id, device_bis_id, iot_device_id, description, is_enable, active) OVERRIDING SYSTEM VALUE VALUES ($did, 'rdsppdev$s',1 , 'rdspdev$s', 'idrdspdlist$s', '',true, true);" >> devadd${a}.sql

let s=s+1
done

echo "total:$s" 
