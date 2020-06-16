if [ "a$1" == "a" ] ; then
#  echo "no arguments"
  exit 1
fi
#echo "ssr://server:port:protocol:method:obfs:password_base64/?params_base64"
#echo "-----------"

strcoded=`echo "$1" | sed "s/-/+/g" | sed "s/_/\//g"`

lencoded=${#strcoded};
remind=$((lencoded%4));

while [ "$remind" != "0" ]
do
	strcoded="$strcoded=";
	remind=$((remind+1));
	remind=$((remind%4));
done


echo $strcoded | openssl base64 -d

#echo " "

