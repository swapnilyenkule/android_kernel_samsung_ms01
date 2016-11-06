#!/bin/sh
# Needed for systems without gettext
<<<<<<< HEAD
$* -xc -o /dev/null - > /dev/null 2>&1 << EOF
=======
$* -x c -o /dev/null - > /dev/null 2>&1 << EOF
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
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

