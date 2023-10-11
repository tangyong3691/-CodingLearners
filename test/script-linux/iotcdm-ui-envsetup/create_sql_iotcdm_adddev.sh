let s=1
a=$1
touch devadd${a}.sql
while [ $s -lt 20000 ] 
do
did=$(( s + 100 ))
echo "INSERT INTO public.device(id, name, device_type_id, device_bis_id, iot_device_id, description, is_enable, active) OVERRIDING SYSTEM VALUE VALUES ($did, 'opcppdev$s',1 , 'opcpdev$s', 'idopcpdlist$s', '',true, true);" >> devadd${a}.sql

let s=s+1
done

echo "total:$s" 
