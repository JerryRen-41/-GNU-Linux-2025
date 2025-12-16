#!/bin/sh

set -e

echo "hello world" > test.txt

MD5_REF=$(md5sum test.txt | awk '{print $1}')
SHA1_REF=$(sha1sum test.txt | awk '{print $1}')

MD5_OUT=$($top_builddir/src/rhasher MD5 test.txt)
SHA1_OUT=$($top_builddir/src/rhasher SHA1 test.txt)

if [ "$MD5_REF" != "$MD5_OUT" ]; then
    echo "MD5 mismatch"
    echo "ref: $MD5_REF"
    echo "got: $MD5_OUT"
    exit 1
fi

if [ "$SHA1_REF" != "$SHA1_OUT" ]; then
    echo "SHA1 mismatch"
    echo "ref: $SHA1_REF"
    echo "got: $SHA1_OUT"
    exit 1
fi

echo "All tests passed."
exit 0

