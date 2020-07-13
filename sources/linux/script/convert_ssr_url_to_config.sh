if [ "a$1" == "a" ] ; then
  echo "no arguments"
  exit 0
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


ssrurl=$1
ssrurl=${ssrurl#*//}

#echo "ssr://server:port:protocol:method:obfs:password_base64/?params_base64"
#echo "-----------"

demain=`$callpath/de_base64_url.sh $ssrurl`

#echo $demain

minfo=${demain%/?*}

serverinfo=${minfo%%:*}
minfo=${minfo#*:}
portinfo=${minfo%%:*}
minfo=${minfo#*:}
protocolinfo=${minfo%%:*}
minfo=${minfo#*:}
methodinfo=${minfo%%:*}
minfo=${minfo#*:}
obfsinfo=${minfo%%:*}

#echo "srv: $serverinfo"
#echo "port: $portinfo"
#echo "protocol: $protocolinfo"
#echo "method: $methodinfo"
#echo "obfs: $obfsinfo"


passwd=${minfo##*:}
passwd=`$callpath/de_base64_url.sh $passwd`
#echo "passwd = $passwd"

parm=${demain#*/?}

while  [ "a$parm" != "a" ] 
do 
thep=${parm%%&*}
argn=${thep%=*}
valn=${thep#*=}
tval=`$callpath/de_base64_url.sh $valn`
if [ "$argn" == "obfsparam" ] ; then
obfsparaminfo=$tval
fi
if [ "$argn" == "protoparam" ] ; then
protoparaminfo=$tval
fi

#echo "$argn = $tval"
parm=${parm#*&}

if [ "$parm" == "$thep" ] ; then
parm=""
fi

done	


# print shadowsocksr config
echo "{"
echo "  \"server\": \"$serverinfo\", "
echo "  \"local_address\": \"127.0.0.1\", "

echo "  \"local_port\": 1083, "
echo "  \"timeout\": 300, "
echo "  \"workers\": 1, "
echo "  \"server_port\": $portinfo, "

echo "  \"password\": \"$passwd\", "
echo "  \"method\": \"$methodinfo\", "
echo "  \"obfs\": \"$obfsinfo\", "
echo "  \"obfs_param\": \"$obfsparaminfo\", "
echo "  \"protocol\": \"$protocolinfo\", "
echo "  \"protocol_param\": \"$protoparaminfo\" "
echo "}"

# end
echo " "

