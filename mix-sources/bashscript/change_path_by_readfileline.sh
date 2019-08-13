echo "file path: $1"

if ! [ -f "$1" ] ; then
echo "file isn't exist"
exit 1
fi

cat $1 | while read line
do
if [ "a${line}" != "a" ]; then
#echo "line: ${line}"
fathdir=$(dirname "${line}")
#echo "fat: ${fathdir}"
echo "mv ${line} ${fathdir}/beidouapp"
mv ${line} ${fathdir}/beidouapp
sdir=$(dirname "${fathdir}")
echo "mv ${fathdir} ${sdir}/com"
mv ${fathdir} ${sdir}/com
fi
 	
done
