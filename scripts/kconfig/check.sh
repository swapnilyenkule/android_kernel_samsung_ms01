#!/bin/sh
# Needed for systems without gettext
<<<<<<< HEAD
$* -xc -o /dev/null - > /dev/null 2>&1 << EOF
=======
$* -x c -o /dev/null - > /dev/null 2>&1 << EOF
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
#include <libintl.h>
int main()
{
	gettext("");
	return 0;
}
EOF
if [ ! "$?" -eq "0"  ]; then
	echo -DKBUILD_NO_NLS;
fi

