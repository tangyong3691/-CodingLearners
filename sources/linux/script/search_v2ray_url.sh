searchfile=$1
searchstring=$2
echo "search file:$searchfile , search string:$searchstring"
if [ "a$searchfile" == "a" ] || [ "a$searchstring" == "a" ] ; then
	echo "err,argument"
	exit 1
fi
if ! [ -e $searchfile ] ; then
	echo "err,search file can't find"
	exit 1
fi

softcmd=$0
softpwd=${softcmd%/*}
#echo "soft:$softpwd"
softname=${softcmd##*/}
softpwdfirst=${softcmd:0:1}
#echo "fri:$softpwdfirst"

if [ "a$softpwd" == "a$softname" ] ; then
	callpath=`which $softname`
elif [ "$softpwdfirst" == "/" ] ; then
	callpath=$softpwd
else
	callpath=$PWD/$softpwd
fi

if ! [ -f  "$callpath/de_base64_url.sh" ] ; then
	echo "Couldn't find :$callpath/de_base64_url.sh"
	exit 1
fi

echo "convert file format linux"
sed -i 's/\r$//g' $searchfile
echo "  "

while read line; do
#	echo "$line"
	messsign=${line%:*}
#	echo "$messsign"
	demessinfo=`$callpath/de_base64_url.sh ${line##*/}`
#	echo $demessinfo
	searched=`echo $demessinfo | grep $searchstring`
	if [ "a$searched" != "a" ] ; then
		echo "$line"
		echo "   $demessinfo"
	fi
done < $searchfile

