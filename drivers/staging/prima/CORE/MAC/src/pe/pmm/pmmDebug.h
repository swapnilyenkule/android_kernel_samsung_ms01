/*
<<<<<<< HEAD
 * Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2012-2013 The Linux Foundation. All rights reserved.
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 *
 * Previously licensed under the ISC license by Qualcomm Atheros, Inc.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
<<<<<<< HEAD
/*
 * Copyright (c) 2012, The Linux Foundation. All rights reserved.
 *
 * Previously licensed under the ISC license by Qualcomm Atheros, Inc.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
=======

/*
 * This file was originally distributed by Qualcomm Atheros, Inc.
 * under proprietary terms before Copyright ownership was assigned
 * to the Linux Foundation.
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 */

/*
 *
<<<<<<< HEAD
 * Airgo Networks, Inc proprietary. All rights reserved.
=======
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 * Author:      Sandesh Goel
 * Date:        02/25/02
 * History:-
 * Date            Modified by    Modification Information
 * --------------------------------------------------------------------
<<<<<<< HEAD
 * 
=======
 *
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 */

#ifndef __PMM_DEBUG_H__
#define __PMM_DEBUG_H__

#include "utilsApi.h"
#include "sirDebug.h"

#define UL_HI( field ) ( *( ( (ULONG *)(&(field)) ) + 1 ) )
#define UL_LO( field ) ( *( ( (ULONG *)(&(field)) ) + 0 ) )

<<<<<<< HEAD

void pmmLog(tpAniSirGlobal pMac, tANI_U32 loglevel, const char *pString,...) ;
=======
#if !defined(__printf)
#define __printf(a,b)
#endif

void __printf(3,4) pmmLog(tpAniSirGlobal pMac, tANI_U32 loglevel,
                          const char *pString, ...) ;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#endif

