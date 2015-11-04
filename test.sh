#!/bin/bash

function test {
  diff <(./mss_tests < test/test$1) <(cat ./test/wzor$1)
}

if [ $# -eq 1 ] 
  then
    test $1
  else
    for i in {1..5}
    do
      test $i
    done
fi
