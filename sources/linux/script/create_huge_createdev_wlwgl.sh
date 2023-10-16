let num=1
let startid=50
while [ $num -lt 501 ] ; do
id=$(( num + startid ))
echo "INSERT INTO public.device(id, name, device_type_id, device_bis_id, iot_device_id, description, is_enable) OVERRIDING SYSTEM VALUE VALUES ($id, 'zzdev$num',1 , 'opcpdev9091$num', 'idopcpdlist9091$num', '',true);"

num=$(( num + 1 ))
done


