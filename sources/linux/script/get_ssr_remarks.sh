if [ "a$1" == "a" ] ; then
  echo "no arguments"
  exit 0
fi

dessru=$1
ssu=${dessru#ssr://}

strcoded=`echo "$ssu" | sed "s/-/+/g" | sed "s/_/\//g"`

lencoded=${#strcoded};
remind=$((lencoded%4));

while [ "$remind" != "0" ]
do
	strcoded="$strcoded=";
	remind=$((remind+1));
	remind=$((remind%4));
done


dss=`echo $strcoded | openssl base64 -d`
#echo $dss

rmk=${dss#*remarks=}

#echo $rmk

emk=${rmk%%&*}

#echo $emk

emk=`echo "$emk" | sed "s/-/+/g" | sed "s/_/\//g"`

mlen=${#emk}
mlea=$((mlen%4))
while [ "$mlea" != "0" ]
do
        emk="$emk="
        mlea=$((mlea+1))
        mlea=$((mlea%4))
done

echo "----"
echo $emk | openssl base64 -d
echo "  "
echo $dessru



