#ifndef _M68K_DIV64_H
#define _M68K_DIV64_H

#ifdef CONFIG_CPU_HAS_NO_MULDIV64
#include <asm-generic/div64.h>
#else

#include <linux/types.h>

/* n = n / base; return rem; */

#define do_div(n, base) ({					\
	union {							\
		unsigned long n32[2];				\
		unsigned long long n64;				\
	} __n;							\
	unsigned long __rem, __upper;				\
<<<<<<< HEAD
=======
	unsigned long __base = (base);				\
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
								\
	__n.n64 = (n);						\
	if ((__upper = __n.n32[0])) {				\
		asm ("divul.l %2,%1:%0"				\
<<<<<<< HEAD
			: "=d" (__n.n32[0]), "=d" (__upper)	\
			: "d" (base), "0" (__n.n32[0]));	\
	}							\
	asm ("divu.l %2,%1:%0"					\
		: "=d" (__n.n32[1]), "=d" (__rem)		\
		: "d" (base), "1" (__upper), "0" (__n.n32[1]));	\
=======
		     : "=d" (__n.n32[0]), "=d" (__upper)	\
		     : "d" (__base), "0" (__n.n32[0]));		\
	}							\
	asm ("divu.l %2,%1:%0"					\
	     : "=d" (__n.n32[1]), "=d" (__rem)			\
	     : "d" (__base), "1" (__upper), "0" (__n.n32[1]));	\
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	(n) = __n.n64;						\
	__rem;							\
})

#endif /* CONFIG_CPU_HAS_NO_MULDIV64 */

#endif /* _M68K_DIV64_H */
