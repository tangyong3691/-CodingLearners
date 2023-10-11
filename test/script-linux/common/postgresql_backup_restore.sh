#!/bin/bash
echo "开始执行 PostgreSql 数据库的备份！"
nowtime=$(date "+%Y%m%d%H%M%S")
export PGPASSWORD='Hw!@#123'
echo "时间：" $nowtime
#set timeout 500
#输入IP 端口、用户名。 备份文件输出的位置及文件名： /home/postgresql_backup/backup"$nowtime"。dump的数据库为 postgres
/usr/pgsql-13/bin/pg_dump -h 192.168.3.246 -p 5432 -U root -F c -b -v -f "/obsfs/backups/postgres-BDH_EE-${nowtime}"  BDH_EE

#when use it below, then this database must exist and matches the structure in the backup file when restore
#/usr/pgsql-13/bin/pg_dump -h 192.168.3.246 -p 5432 -U root -C -f "/obsfs/backups/postgres-BDH_EE-${nowtime}"  BDH_EE
echo "数据库备份结束！"
echo "开始附件备份"
tar czvf  "/obsfs/backups/data${nowtime}.tgz" /home/njcxzx/bdh-data
tar czvf  "/obsfs/backups/files${nowtime}.tgz" /obsfs/bdh-files
#rsync -aExvh /obsfs/bdh-files /obsfs/backups/bdh-files__sync
sleep 1
echo "附件备份完成!"
exit;
###restore command examp####
##attentsion: no check it, should only example
echo "resotre db BDH_EE"
/usr/pgsql-13/bin/dropdb -h 192.168.3.246 -p 5432 -U njcx BDH_EE
/usr/pgsql-13/bin/pg_restore -h 192.168.3.246 -p 5432 -C -U root -d postgres < postgres-BDH_EE-20221029210826
