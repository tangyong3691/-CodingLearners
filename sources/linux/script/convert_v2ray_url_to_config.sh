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

messurl=$1
messurl=${messurl#*//}
demain=`$callpath/de_base64_url.sh $messurl`

#echo $demain

chkit=`echo $demain | grep "id"`
if [ "a$chkit" == "a" ] ; then
	echo "err, decode result: $demian"
	exit 1
fi

leftinf=$demain
leftinf=${leftinf%\}*}
leftinf=${leftinf#*\{}
#echo "this: $leftinf"


while true ;
do
tinf=${leftinf##*,}

tsign=${tinf%:*}
tsign=${tsign#*\{}
tsign=${tsign#*\"}
tsign=${tsign%\"*}

tval=${tinf#*:}
#echo "$tval"
#tval="${tval#\"}"
#tval="${tavl%\"}"
##echo "tsignnnn:$tsign"
##echo "tvallll:$tval"

tval=`sed -e 's/^"//' -e 's/"$//' <<<"$tval"`

#echo "$tval"

if [ "$tsign" == "add" ] ; then
	inf_add=$tval
elif [ "$tsign" == "aid" ] ; then
	inf_aid=$tval
elif [ "$tsign" == "id" ] ; then
	inf_id=$tval
elif [ "$tsign" == "net" ] ; then
	inf_net=$tval
elif [ "$tsign" == "path" ] ; then
	inf_path=$tval
elif [ "$tsign" == "port" ] ; then
	inf_port=$tval
elif [ "$tsign" == "tls" ] ; then
	inf_tls=$tval
fi


leftinf=${leftinf%,*}
if [ "$tinf" == "$leftinf" ] ; then
break;
fi
done

#echo "-----------"
#echo "$inf_add"
#echo "$inf_aid"
#echo "$inf_id"
#echo "$inf_net"
#echo "$inf_path"
#echo "$inf_port"
#echo "$inf_tls"

#echo "----start ouput config---"

echo "{"
echo '  "inbound": {'
echo "    \"port\": 1083,"
echo '    "listen": "127.0.0.1",'
echo '    "protocol": "socks",'
echo '    "sniffing": {'
echo '        "enabled": true,'
echo '        "destOverride": ["http", "tls"]'
echo '    },'
echo '    "settings": {'
echo '      "auth": "noauth",'
echo '      "udp": false,'
echo '      "ip": "127.0.0.1"'
echo '    }'
echo '  },'
echo '  "outbound": {'
echo '    "protocol": "vmess",'
echo '    "settings": {'
echo '      "vnext": ['
echo '        {'
echo "          \"address\": \"$inf_add\","
echo "          \"port\": $inf_port,"
echo '          "users": ['
echo '            {'
echo "              \"id\": \"$inf_id\","
echo "              \"alterId\": $inf_aid"
echo '            }'
echo '          ]'
echo '        }'
echo '      ]'
echo '    },'
echo '    "streamSettings": {'
echo "      \"network\": \"$inf_net\","
echo "      \"security\": \"$inf_tls\","
echo '      "wsSettings": {'
echo '         "connectionReuse": false,'
echo "         \"path\": \"$inf_path\""
echo '      }'
echo '    },'
echo '    "tag": "direct"'
echo '  },'
echo '  "policy": {'
echo '    "levels": {'
echo '      "0": {'
echo '        "uplinkOnly": 0'
echo '      }'
echo '    }'
echo '  },'
echo '  "dns": {'
echo '     "servers": ['
echo '         "8.8.8.8",'
echo '         "8.8.4.4",'
echo '         "localhost"'
echo '      ]'
echo '  }'
echo '}'

