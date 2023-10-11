#!/bin/sh
num=1
startid=10000
while [ $num -lt 5001 ] ; do
id=$(( num + startid ))
echo "INSERT INTO public.device(id, name, device_type_id, device_bis_id, iot_device_id, description, is_enable, active) OVERRIDING SYSTEM VALUE VALUES ($id, 'zyzdev$num',1 , 'zydev909a$num', 'idzylist909a$num', '',true,true);"

num=$(( num + 1 ))
done


