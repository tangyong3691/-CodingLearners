#!/bin/sh

dbname="your_db"
jtt808cmd="truncate table jtt808_device_location_track;truncate table jtt808_device_alarm_track;"
mysql -uroot -p123456789123 $dbname -e "$jtt808cmd"

