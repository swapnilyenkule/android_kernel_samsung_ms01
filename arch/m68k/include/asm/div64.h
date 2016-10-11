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
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
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
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	(n) = __n.n64;						\
	__rem;							\
})

#endif /* CONFIG_CPU_HAS_NO_MULDIV64 */

#endif /* _M68K_DIV64_H */
