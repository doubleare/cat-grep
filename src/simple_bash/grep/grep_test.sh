#!/bin/bash

SUCCESS=0
FAIL=0
DIFF_RES=""

for va in -e -v -c -l -n -h -i -o
do
  TEST1="$var int s21_grep.c s21_grep.h"
  echo "$TEST1"
  ./s21_grep $TEST1 > s21_grep.txt
  grep $TEST1 > grep.txt
  DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
  if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
    then
      (( SUCCESS++ ))
    else
      (( FAIL++ ))
  fi
  rm s21_grep.txt grep.txt
done

for va in -v -c -l -n -h -i -o
do
  for va1 in -v -l -n -h -i -o
  do
    if [ $va != $va1 ]
    then
      TEST1="$va $va1 int s21_grep.c s21_grep.h"
      echo "$TEST1"
      ./s21_grep $TEST1 > s21_grep.txt
      grep $TEST1 > grep.txt
      DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
      if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
        then
          (( SUCCESS++ ))
        else
          (( FAIL++ ))
      fi
      rm s21_grep.txt grep.txt
    fi
  done
done

for var in -v -c -l -n -h -i -o
do
  for var2 in -v -c -l -n -h -i -o
  do
      for var3 in -v -c -l -n -h -i -o
      do
        if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ]
        then
            TEST1="-e for -f ../common/pattern.txt $var $var2 $var3 s21_grep.c s21_grep.h"
            echo "$TEST1"
            ./s21_grep $TEST1 > s21_grep.txt
            grep $TEST1 > grep.txt
            DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
            if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
              then
                (( SUCCESS++ ))
              else
                (( FAIL++ ))
            fi
            rm s21_grep.txt grep.txt

            TEST2="-e for -f ../common/pattern.txt $var $var2 $var3 s21_grep.c"
            echo "$TEST2"
            ./s21_grep $TEST2 > s21_grep.txt
            grep $TEST2 > grep.txt
            DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
            if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
              then
                (( SUCCESS++ ))
              else
                (( FAIL++ ))
            fi
            rm s21_grep.txt grep.txt

            TEST3="-e ^For -f ../common/pattern.txt $var $var2 $var3 s21_grep.c s21_grep.h"
            echo "$TEST3"
            ./s21_grep $TEST3 > s21_grep.txt
            grep $TEST3 > grep.txt
            DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
            if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
              then
                (( SUCCESS++ ))
              else
                (( FAIL++ ))
            fi
            rm s21_grep.txt grep.txt

            TEST4=" -e ^FoR -f ../common/pattern.txt $var $var2 $var3 s21_grep.c"
            echo "$TEST4"
            ./s21_grep $TEST4 > s21_grep.txt
            grep $TEST4 > grep.txt
            DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
            if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
              then
                (( SUCCESS++ ))
              else
                (( FAIL++ ))
            fi
            rm s21_grep.txt grep.txt

            TEST5=" -e "[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}" -f ../common/pattern.txt $var $var2 $var3 ../common/text.txt"
            echo "$TEST5"
            ./s21_grep $TEST5 > s21_grep.txt
            grep $TEST5 > grep.txt
            DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
            if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
              then
                (( SUCCESS++ ))
              else
                (( FAIL++ ))
            fi
            rm s21_grep.txt grep.txt
          fi
      done
  done
done

echo "SUCCESS: $SUCCESS"
echo "FAIL: $FAIL"

if (( $FAIL != 0 ));
then
  exit 1
fi