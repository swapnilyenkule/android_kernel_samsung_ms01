/*
 * FIPS 200 support.
 *
 * Copyright (c) 2008 Neil Horman <nhorman@tuxdriver.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include "internal.h"

<<<<<<< HEAD
// change@wtl.kSingh - In FIPS mode always enable this flag - starts
#ifdef CONFIG_CRYPTO_FIPS
int fips_enabled = 1;
#else
int fips_enabled;
#endif
// change@wtl.kSingh - In FIPS mode always enable this flag - ends

=======
int fips_enabled;
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
EXPORT_SYMBOL_GPL(fips_enabled);

/* Process kernel command-line parameter at boot time. fips=0 or fips=1 */
static int fips_enable(char *str)
{
	fips_enabled = !!simple_strtol(str, NULL, 0);
	printk(KERN_INFO "fips mode: %s\n",
		fips_enabled ? "enabled" : "disabled");
	return 1;
}

__setup("fips=", fips_enable);
