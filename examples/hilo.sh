#!/bin/bash
# sample hi-lo game shell script
echo You think of a number from 1 to 1024 and I will guess it.
TRIES=1
HI=1024
LO=1
while true
do
  GUESS=`expr \( $HI + $LO \) / 2`
  echo Is it $GUESS\?
  echo -n 'Please type "h", for high "l" for low or "c" for correct: '
  read RESPONSE
  if [ "$RESPONSE" = "h" ]
  then
    TRIES=`expr $TRIES + 1`
    HI=$GUESS
    continue
  fi
  if [ "$RESPONSE" = "l" ]
  then
    TRIES=`expr $TRIES + 1`
    LO=$GUESS
    continue
  fi
  if [ "$RESPONSE" = "c" ]
  then
    echo I GOT IT RIGHT IN $TRIES TRIES
    exit 0
  fi
done
