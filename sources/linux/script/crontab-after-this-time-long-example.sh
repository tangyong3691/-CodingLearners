shuttime=$1
if [ "a$shuttime" == "a" ] ; then
echo "err: please with arg number (hour), or hour:minute"
echo "     when after a  period of this long time , then shutdown"
echo "     so, wait total minutes = {h} * 60 + {m}"
exit 0
fi

if ! [[ $shuttime =~ ^([0-9]{0,3}:){1}[0-9]{1,2}$ ]] ; then 
echo "arg err"
exit 0
fi

endhour=${shuttime%:*}
endminute=${shuttime#*:}
if [ "$endminute" == "$endhour" ] ; then
endminute="0"
fi

if [ "a$endhour" == "a" ] ; then
endhour="0"
fi

if ! [[ "$endhour" =~ ^[0-9]{1,2}$ ]] ; then
echo "arg err2"
exit 0
fi

## this cal don't allow space, like:
##  (wrong) = $((endhour)) * 60 + $((endminute))
let totalmin=$((endhour))*60+$((endminute))

echo "delay $endhour : $endminute"
echo "tota delay minutes: $totalmin"


curseconds=`date +%s`
delayto=$(( ($((curseconds)) / 60 + $((totalmin)))*60 ))
#echo "$delayto"

totime=`date --date="@$delayto"`
current_human_tm=`date --date="@$curseconds"`
echo "start now time = $current_human_tm"
echo " $totime"

#exit 0

ntime=0
while [ $ntime -lt $delayto ] 
do
sleep 50
newseconds=`date +%s`
ntime=$((newseconds))
done
qtime=`date`
echo "time is:  $qtime"
echo "  starting task now"
sleep 5

# running this task
#sudo shutdown now
echo "please insert task below"

