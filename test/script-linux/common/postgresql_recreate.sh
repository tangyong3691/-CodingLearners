#!/bin/bash
export PGPASSWORD='Hw!@#12320221025'
/usr/pgsql-13/bin/dropdb --if-exists --host=192.168.3.246 --username=njcx BDH_EE
/usr/pgsql-13/bin/createdb --locale=en_US.UTF-8 --encoding=UTF8 --host=192.168.3.246 --username=njcx --owner=njcx BDH_EE

