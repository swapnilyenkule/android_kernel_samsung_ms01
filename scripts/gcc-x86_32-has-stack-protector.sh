#!/bin/sh

<<<<<<< HEAD
echo "int foo(void) { char X[200]; return 3; }" | $* -S -xc -c -O0 -fstack-protector - -o - 2> /dev/null | grep -q "%gs"
=======
echo "int foo(void) { char X[200]; return 3; }" | $* -S -x c -c -O0 -fstack-protector - -o - 2> /dev/null | grep -q "%gs"
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
if [ "$?" -eq "0" ] ; then
	echo y
else
	echo n
fi
