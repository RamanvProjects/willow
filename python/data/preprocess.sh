#! /bin/sh

for f in $(ls json/);
do 
echo "Processing $f..."
cat "json/$f" | sed 's/(/\"/g' | sed 's/)/\"/g' | sed 's/ //g' | sed -e ':a' -e 'N' -e '$!ba' -e 's/\n/,/g' | sed 's/{,/{/g' | sed 's/,}/}/g' > "json/$f"
done
