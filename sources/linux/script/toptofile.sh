while [ -f /opt/tools/maintenance/top_running ]
do


echo '-----------------' >> top_log_1
ps -ef >> top_log_1
echo '+++++++++' >> top_log_1
echo '+++' >> top_log_1
lsof +D /opt/csvn_data/data/repositories  >> top_log_1
echo '+--+--+--' >> top_log_1

top -b -d 3 -n 3 | grep 'top -' -A 17 >> top_log_1
sleep 10

done
