/*
<<<<<<< HEAD
 * Copyright (C) 1999-2014, Broadcom Corporation
=======
 * Copyright (C) 1999-2015, Broadcom Corporation
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 * 
 *      Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2 (the "GPL"),
 * available at http://www.broadcom.com/licenses/GPLv2.php, with the
 * following added to such license:
 * 
 *      As a special exception, the copyright holders of this software give you
 * permission to link this software with independent modules, and to copy and
 * distribute the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and conditions of
 * the license of that module.  An independent module is a module which is not
 * derived from this software.  The special exception does not apply to any
 * modifications of the software.
 * 
 *      Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a license
 * other than the GPL, without Broadcom's express prior written consent.
<<<<<<< HEAD
 * $Id: typedefs.h 397286 2013-04-18 01:42:19Z $
=======
 * $Id: typedefs.h 453696 2014-02-06 01:10:20Z $
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 */

#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

#ifdef SITE_TYPEDEFS

<<<<<<< HEAD

=======
/*
 * Define SITE_TYPEDEFS in the compile to include a site-specific
 * typedef file "site_typedefs.h".
 *
 * If SITE_TYPEDEFS is not defined, then the code section below makes
 * inferences about the compile environment based on defined symbols and
 * possibly compiler pragmas.
 *
 * Following these two sections is the Default Typedefs section.
 * This section is only processed if USE_TYPEDEF_DEFAULTS is
 * defined. This section has a default set of typedefs and a few
 * preprocessor symbols (TRUE, FALSE, NULL, ...).
 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#include "site_typedefs.h"

#else

<<<<<<< HEAD

=======
/*
 * Infer the compile environment based on preprocessor symbols and pragmas.
 * Override type definitions as needed, and include configuration-dependent
 * header files to define types.
 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#ifdef __cplusplus

#define TYPEDEF_BOOL
#ifndef FALSE
#define FALSE	false
#endif
#ifndef TRUE
#define TRUE	true
#endif

<<<<<<< HEAD
#else	


#endif	

#if defined(__x86_64__)
=======
#else	/* ! __cplusplus */


#endif	/* ! __cplusplus */

#if defined(__LP64__)
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#define TYPEDEF_UINTPTR
typedef unsigned long long int uintptr;
#endif





#if defined(_NEED_SIZE_T_)
typedef long unsigned int size_t;
#endif





#if defined(__sparc__)
#define TYPEDEF_ULONG
#endif


<<<<<<< HEAD

=======
/*
 * If this is either a Linux hybrid build or the per-port code of a hybrid build
 * then use the Linux header files to get some of the typedefs.  Otherwise, define
 * them entirely in this file.  We can't always define the types because we get
 * a duplicate typedef error; there is no way to "undefine" a typedef.
 * We know when it's per-port code because each file defines LINUX_PORT at the top.
 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#if !defined(LINUX_HYBRID) || defined(LINUX_PORT)
#define TYPEDEF_UINT
#ifndef TARGETENV_android
#define TYPEDEF_USHORT
#define TYPEDEF_ULONG
<<<<<<< HEAD
#endif 
=======
#endif /* TARGETENV_android */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#ifdef __KERNEL__
#include <linux/version.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 19))
#define TYPEDEF_BOOL
<<<<<<< HEAD
#endif	

=======
#endif	/* >= 2.6.19 */
/* special detection for 2.6.18-128.7.1.0.1.el5 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#if (LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 18))
#include <linux/compiler.h>
#ifdef noinline_for_stack
#define TYPEDEF_BOOL
#endif
<<<<<<< HEAD
#endif	
#endif	
#endif  

=======
#endif	/* == 2.6.18 */
#endif	/* __KERNEL__ */
#endif  /* !defined(LINUX_HYBRID) || defined(LINUX_PORT) */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68




<<<<<<< HEAD
#if defined(__GNUC__) && defined(__STRICT_ANSI__)
#define TYPEDEF_INT64
#define TYPEDEF_UINT64
#endif 


=======
/* Do not support the (u)int64 types with strict ansi for GNU C */
#if defined(__GNUC__) && defined(__STRICT_ANSI__)
#define TYPEDEF_INT64
#define TYPEDEF_UINT64
#endif /* defined(__GNUC__) && defined(__STRICT_ANSI__) */

/* ICL accepts unsigned 64 bit type only, and complains in ANSI mode
 * for signed or unsigned
 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#if defined(__ICL)

#define TYPEDEF_INT64

#if defined(__STDC__)
#define TYPEDEF_UINT64
#endif

<<<<<<< HEAD
#endif 

#if !defined(__DJGPP__)


#if defined(__KERNEL__)


#if !defined(LINUX_HYBRID) || defined(LINUX_PORT)
#include <linux/types.h>	
#endif 
=======
#endif /* __ICL */

#if !defined(__DJGPP__)

/* pick up ushort & uint from standard types.h */
#if defined(__KERNEL__)

/* See note above */
#if !defined(LINUX_HYBRID) || defined(LINUX_PORT)
#include <linux/types.h>	/* sys/types.h and linux/types.h are oil and water */
#endif /* !defined(LINUX_HYBRID) || defined(LINUX_PORT) */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#else


#include <sys/types.h>

<<<<<<< HEAD
#endif 
=======
#endif /* linux && __KERNEL__ */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#endif 



<<<<<<< HEAD

#define USE_TYPEDEF_DEFAULTS

#endif 



=======
/* use the default typedefs in the next section of this file */
#define USE_TYPEDEF_DEFAULTS

#endif /* SITE_TYPEDEFS */


/*
 * Default Typedefs
 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#ifdef USE_TYPEDEF_DEFAULTS
#undef USE_TYPEDEF_DEFAULTS

#ifndef TYPEDEF_BOOL
<<<<<<< HEAD
typedef	 unsigned char	bool;
#endif


=======
typedef	/* @abstract@ */ unsigned char	bool;
#endif

/* define uchar, ushort, uint, ulong */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#ifndef TYPEDEF_UCHAR
typedef unsigned char	uchar;
#endif

#ifndef TYPEDEF_USHORT
typedef unsigned short	ushort;
#endif

#ifndef TYPEDEF_UINT
typedef unsigned int	uint;
#endif

#ifndef TYPEDEF_ULONG
typedef unsigned long	ulong;
#endif

<<<<<<< HEAD

=======
/* define [u]int8/16/32/64, uintptr */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#ifndef TYPEDEF_UINT8
typedef unsigned char	uint8;
#endif

#ifndef TYPEDEF_UINT16
typedef unsigned short	uint16;
#endif

#ifndef TYPEDEF_UINT32
typedef unsigned int	uint32;
#endif

#ifndef TYPEDEF_UINT64
typedef unsigned long long uint64;
#endif

#ifndef TYPEDEF_UINTPTR
typedef unsigned int	uintptr;
#endif

#ifndef TYPEDEF_INT8
typedef signed char	int8;
#endif

#ifndef TYPEDEF_INT16
typedef signed short	int16;
#endif

#ifndef TYPEDEF_INT32
typedef signed int	int32;
#endif

#ifndef TYPEDEF_INT64
typedef signed long long int64;
#endif

<<<<<<< HEAD

=======
/* define float32/64, float_t */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#ifndef TYPEDEF_FLOAT32
typedef float		float32;
#endif

#ifndef TYPEDEF_FLOAT64
typedef double		float64;
#endif

<<<<<<< HEAD

=======
/*
 * abstracted floating point type allows for compile time selection of
 * single or double precision arithmetic.  Compiling with -DFLOAT32
 * selects single precision; the default is double precision.
 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#ifndef TYPEDEF_FLOAT_T

#if defined(FLOAT32)
typedef float32 float_t;
<<<<<<< HEAD
#else 
typedef float64 float_t;
#endif

#endif 


=======
#else /* default to double precision floating point */
typedef float64 float_t;
#endif

#endif /* TYPEDEF_FLOAT_T */

/* define macro values */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#ifndef FALSE
#define FALSE	0
#endif

#ifndef TRUE
<<<<<<< HEAD
#define TRUE	1  
=======
#define TRUE	1  /* TRUE */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#endif

#ifndef NULL
#define	NULL	0
#endif

#ifndef OFF
#define	OFF	0
#endif

#ifndef ON
<<<<<<< HEAD
#define	ON	1  
#endif

#define	AUTO	(-1) 


=======
#define	ON	1  /* ON = 1 */
#endif

#define	AUTO	(-1) /* Auto = -1 */

/* define PTRSZ, INLINE */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#ifndef PTRSZ
#define	PTRSZ	sizeof(char*)
#endif


<<<<<<< HEAD

=======
/* Detect compiler type. */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#if defined(__GNUC__) || defined(__lint)
	#define BWL_COMPILER_GNU
#elif defined(__CC_ARM) && __CC_ARM
	#define BWL_COMPILER_ARMCC
#else
	#error "Unknown compiler!"
#endif 


#ifndef INLINE
	#if defined(BWL_COMPILER_MICROSOFT)
		#define INLINE __inline
	#elif defined(BWL_COMPILER_GNU)
		#define INLINE __inline__
	#elif defined(BWL_COMPILER_ARMCC)
		#define INLINE	__inline
	#else
		#define INLINE
	#endif 
<<<<<<< HEAD
#endif 
=======
#endif /* INLINE */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#undef TYPEDEF_BOOL
#undef TYPEDEF_UCHAR
#undef TYPEDEF_USHORT
#undef TYPEDEF_UINT
#undef TYPEDEF_ULONG
#undef TYPEDEF_UINT8
#undef TYPEDEF_UINT16
#undef TYPEDEF_UINT32
#undef TYPEDEF_UINT64
#undef TYPEDEF_UINTPTR
#undef TYPEDEF_INT8
#undef TYPEDEF_INT16
#undef TYPEDEF_INT32
#undef TYPEDEF_INT64
#undef TYPEDEF_FLOAT32
#undef TYPEDEF_FLOAT64
#undef TYPEDEF_FLOAT_T

<<<<<<< HEAD
#endif 


#define UNUSED_PARAMETER(x) (void)(x)


#define DISCARD_QUAL(ptr, type) ((type *)(uintptr)(ptr))


#include <bcmdefs.h>
#endif 
=======
#endif /* USE_TYPEDEF_DEFAULTS */

/* Suppress unused parameter warning */
#define UNUSED_PARAMETER(x) (void)(x)

/* Avoid warning for discarded const or volatile qualifier in special cases (-Wcast-qual) */
#define DISCARD_QUAL(ptr, type) ((type *)(uintptr)(ptr))

/*
 * Including the bcmdefs.h here, to make sure everyone including typedefs.h
 * gets this automatically
*/
#include <bcmdefs.h>
#endif /* _TYPEDEFS_H_ */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
