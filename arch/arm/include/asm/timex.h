/*
 *  arch/arm/include/asm/timex.h
 *
 *  Copyright (C) 1997,1998 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Architecture Specific TIME specifications
 */
#ifndef _ASMARM_TIMEX_H
#define _ASMARM_TIMEX_H

#include <mach/timex.h>

typedef unsigned long cycles_t;
<<<<<<< HEAD
#define get_cycles()	({ cycles_t c; read_current_timer(&c) ? 0 : c; })
=======

static inline cycles_t get_cycles (void)
{
	return 0;
}
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4

#endif
