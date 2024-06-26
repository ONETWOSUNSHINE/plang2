#!/bin/sh

CUR_DIR=$1
BIN_DIR=$2
TEST=$3

shift 3
echo "[ Test   ]" `basename $TEST`

ORIG=`mktemp`
PP=`mktemp`
TEMP=`mktemp`
NEWTEST=`mktemp`

$BIN_DIR/plang -past -k $@ $TEST |sort > $ORIG
$BIN_DIR/plang -pp -k $@ $TEST > $TEMP
$BIN_DIR/plang -past -k $@ $TEMP |sort > $PP

DIFFRESULT="`diff -u $ORIG $PP`"
rm $ORIG $TEMP $PP $NEWTEST

if test -z $DIFFRESULT
then
    echo -e "[ \033[0;32mPassed\033[0m ]"
    exit 0
else
    echo -e "[ \033[0;31mFailed\033[0m ]" "$DIFFRESULT"
    exit 1
fi
