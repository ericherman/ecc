#!/bin/bash

function build {
  echo "make clean:"
  make clean
  echo "make:"
  make
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
    echo "FAIL: $RESULT != ${3}"
    exit 1;
  fi
  rm functional-test-${1}*
}

build
check 1 "42" 42
check 2 "0" 0
check 3 "23" 23
check 4 "1+1" 2
check 5 "7+ 23" 30
echo "SUCCESS"
