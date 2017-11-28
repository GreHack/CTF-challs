#!/bin/bash
tmpDir="/tmp/compile-$$"
mkdir -p "$tmpDir"
interm_result=$tmpDir/result
name=${1%.*}
result=$tmpDir/"$name.js"

solc "$1" --bin --abi > $interm_result

echo -n "${name}Info = {code:" > $result
echo -n "\"0x" >> $result
sed -n "/Binary:/{n;p;}" $interm_result >>  $result
perl -pi -e 'chomp if eof' $result
echo -n "\", abi:" >> $result
sed -n "/Contract JSON ABI/{n;p;}" $interm_result >> $result
perl -pi -e 'chomp if eof' $result
echo "}" >> $result

cat $result
