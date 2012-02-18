#!/bin/bash

function build {
  echo "make clean:" &&
  make clean &&
  echo "make:" &&
  make &&
  echo "make check:" &&
  make check
}

function check {
  echo ${1}
  cat > functional-test-${1}.src << EOF
${2}
EOF
  # echo "compile functional-test-${1} sources"
  ./ecc functional-test-${1}.src functional-test-${1}
  # echo "execute and compare"
  ./functional-test-${1}
  RESULT=$?
  if [ $RESULT != ${3} ]
  then
    cat functional-test-${1}.src
    echo "FAIL: $RESULT != ${3}"
    exit 1;
  fi
  rm functional-test-${1}*
}

build || exit $?

check 0 "0" 0
check 1 "42" 42
check 2 "7+ 23" 30
check 3 "-16 - -24 + 3" 11
check 4 "1000000 + 13 - 500000 - 500000" 13
check 5 "1 + 2 * 3 - 4 / (1 +1)" 5
check 6 "-(1 - 16)" 15
#check 7 "int a; a= 3; a" 3

echo "SUCCESS"
