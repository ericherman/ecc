#!/bin/bash
echo "make clean:"
make clean
echo "make:"
make
echo ""
cat > functional-test-1.src << EOF
42
EOF
echo "compile functional-test-1 sources"
./ecc functional-test-1.src -o functional-test-1
echo "execute and compare"
./functional-test-1
if [ $? != 42 ]
then
 echo "FAIL: $? != 42"
 exit 1;
fi
rm functional-test-1*
echo "SUCCESS"
