#!/bin/bash

function build {
  echo "make clean:"
  make clean
  echo "make:"
  make
  echo "make check:"
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

build

check 1 "42" 42
check 2 "0" 0
check 3 "7+ 23" 30
check 4 "10 + 20 + 30" 60
check 5 "16 + 4 - 3" 17
check 6 "1000000 + 13 - 500000 - 500000" 13
check 7 "1 + 2 * 3 - 4 / (1 +1)" 5

echo "SUCCESS"
