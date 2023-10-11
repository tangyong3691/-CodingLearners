#!/bin/bash
echo "warning: delete postgres database BDH_EE"
export PGPASSWORD='Hw!@#12320221025'
while true; do
    read -p "Do you wish to delete database? " yn
    case $yn in
        [Yy]* ) /usr/pgsql-13/bin/dropdb --if-exists --host=192.168.3.246 --username=njcx BDH_EE; break;;
        [Nn]* ) echo "you select no,then exit"; exit;;
        * ) echo "Please answer yes or no.";;
    esac
done
exit;

export PGPASSWORD='Hw!@#12320221025'
/usr/pgsql-13/bin/dropdb --if-exists --host=192.168.3.246 --username=njcx BDH_EE
#/usr/pgsql-13/bin/createdb  --host=192.168.3.246 --username=njcx --owner=njcx BDH_EE

