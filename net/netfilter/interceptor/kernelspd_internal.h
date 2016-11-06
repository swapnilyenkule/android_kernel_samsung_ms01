/**
   @copyright
<<<<<<< HEAD
   Copyright (c) 2013, INSIDE Secure Oy. All rights reserved.
=======
   Copyright (c) 2013 - 2014, INSIDE Secure Oy. All rights reserved.
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
*/


#ifndef KERNELSPD_INTERNAL_H
#define KERNELSPD_INTERNAL_H

#include "kernelspd_defs.h"

#include "kernelspd_command.h"
#include "ip_selector_db.h"
<<<<<<< HEAD
#include "spd_util.h"
=======
#include "ipsec_boundary.h"
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#include "implementation_defs.h"

#define __DEBUG_MODULE__ kernelspdlinux

int
spd_hooks_init(
        void);

void
spd_hooks_uninit(
        void);


int
spd_proc_init(
        void);

void
spd_proc_uninit(
        void);

extern struct IPSelectorDb spd;

extern rwlock_t spd_lock;

<<<<<<< HEAD
=======
extern char *ipsec_boundary;

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#endif /* KERNELSPD_INTERNAL_H */
