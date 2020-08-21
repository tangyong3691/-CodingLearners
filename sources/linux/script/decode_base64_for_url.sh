### ssr url fromat
### ssr://server:port:protocol:method:obfs:password_base64/?params_base64

if [ "a$1" == "a" ] ; then
echo "need argument";
exit
fi

b64str=$1
b64str=${b64str#*://}

strcoded=`echo "$b64str" | sed 's/-/+/g' | sed 's/_/\//g'`;

lencoded=${#strcoded};
remind=$((lencoded%4));

while [ "$remind" != "0" ]
do

strcoded="$strcoded=";
remind=$((remind+1));
remind=$((remind%4));

done

echo $strcoded | openssl base64 -d
echo " "

