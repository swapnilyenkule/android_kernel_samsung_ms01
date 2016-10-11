#!/bin/sh

<<<<<<< HEAD
echo "int foo(void) { char X[200]; return 3; }" | $* -S -xc -c -O0 -fstack-protector - -o - 2> /dev/null | grep -q "%gs"
=======
echo "int foo(void) { char X[200]; return 3; }" | $* -S -x c -c -O0 -fstack-protector - -o - 2> /dev/null | grep -q "%gs"
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
if [ "$?" -eq "0" ] ; then
	echo y
else
	echo n
fi
