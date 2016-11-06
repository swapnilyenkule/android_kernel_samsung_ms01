#ifndef _LINUX_SECCOMP_H
#define _LINUX_SECCOMP_H

<<<<<<< HEAD
#include <linux/compiler.h>
#include <linux/types.h>


/* Valid values for seccomp.mode and prctl(PR_SET_SECCOMP, <mode>) */
#define SECCOMP_MODE_DISABLED	0 /* seccomp is not in use. */
#define SECCOMP_MODE_STRICT	1 /* uses hard-coded filter. */
#define SECCOMP_MODE_FILTER	2 /* uses user-supplied filter. */

/* Valid operations for seccomp syscall. */
#define SECCOMP_SET_MODE_STRICT	0
#define SECCOMP_SET_MODE_FILTER	1

/* Valid flags for SECCOMP_SET_MODE_FILTER */
#define SECCOMP_FILTER_FLAG_TSYNC	1

/*
 * All BPF programs must return a 32-bit value.
 * The bottom 16-bits are for optional return data.
 * The upper 16-bits are ordered from least permissive values to most.
 *
 * The ordering ensures that a min_t() over composed return values always
 * selects the least permissive choice.
 */
#define SECCOMP_RET_KILL	0x00000000U /* kill the task immediately */
#define SECCOMP_RET_TRAP	0x00030000U /* disallow and force a SIGSYS */
#define SECCOMP_RET_ERRNO	0x00050000U /* returns an errno */
#define SECCOMP_RET_TRACE	0x7ff00000U /* pass to a tracer or disallow */
#define SECCOMP_RET_ALLOW	0x7fff0000U /* allow */

/* Masks for the return value sections. */
#define SECCOMP_RET_ACTION	0x7fff0000U
#define SECCOMP_RET_DATA	0x0000ffffU

/**
 * struct seccomp_data - the format the BPF program executes over.
 * @nr: the system call number
 * @arch: indicates system call convention as an AUDIT_ARCH_* value
 *        as defined in <linux/audit.h>.
 * @instruction_pointer: at the time of the system call.
 * @args: up to 6 system call arguments always stored as 64-bit values
 *        regardless of the architecture.
 */
struct seccomp_data {
	int nr;
	__u32 arch;
	__u64 instruction_pointer;
	__u64 args[6];
};

#ifdef __KERNEL__

#define SECCOMP_FILTER_FLAG_MASK	(SECCOMP_FILTER_FLAG_TSYNC)
=======
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#ifdef CONFIG_SECCOMP

#include <linux/thread_info.h>
#include <asm/seccomp.h>

<<<<<<< HEAD
struct seccomp_filter;
/**
 * struct seccomp - the state of a seccomp'ed process
 *
 * @mode:  indicates one of the valid values above for controlled
 *         system calls available to a process.
 * @filter: must always point to a valid seccomp-filter or NULL as it is
 *          accessed without locking during system call entry.
 *
 *          @filter must only be accessed from the context of current as there
 *          is no read locking.
 */
struct seccomp {
	int mode;
	struct seccomp_filter *filter;
};

extern int __secure_computing(int);
static inline int secure_computing(int this_syscall)
{
	if (unlikely(test_thread_flag(TIF_SECCOMP)))
		return  __secure_computing(this_syscall);
	return 0;
}

/* A wrapper for architectures supporting only SECCOMP_MODE_STRICT. */
static inline void secure_computing_strict(int this_syscall)
{
	BUG_ON(secure_computing(this_syscall) != 0);
}

extern long prctl_get_seccomp(void);
extern long prctl_set_seccomp(unsigned long, char __user *);

static inline int seccomp_mode(struct seccomp *s)
=======
typedef struct { int mode; } seccomp_t;

extern void __secure_computing(int);
static inline void secure_computing(int this_syscall)
{
	if (unlikely(test_thread_flag(TIF_SECCOMP)))
		__secure_computing(this_syscall);
}

extern long prctl_get_seccomp(void);
extern long prctl_set_seccomp(unsigned long);

static inline int seccomp_mode(seccomp_t *s)
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
{
	return s->mode;
}

#else /* CONFIG_SECCOMP */

#include <linux/errno.h>

<<<<<<< HEAD
struct seccomp { };
struct seccomp_filter { };

static inline int secure_computing(int this_syscall) { return 0; }
static inline void secure_computing_strict(int this_syscall) { return; }
=======
typedef struct { } seccomp_t;

#define secure_computing(x) do { } while (0)
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

static inline long prctl_get_seccomp(void)
{
	return -EINVAL;
}

<<<<<<< HEAD
static inline long prctl_set_seccomp(unsigned long arg2, char __user *arg3)
=======
static inline long prctl_set_seccomp(unsigned long arg2)
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
{
	return -EINVAL;
}

<<<<<<< HEAD
static inline int seccomp_mode(struct seccomp *s)
{
	return 0;
}
#endif /* CONFIG_SECCOMP */

#ifdef CONFIG_SECCOMP_FILTER
extern void put_seccomp_filter(struct task_struct *tsk);
extern void get_seccomp_filter(struct task_struct *tsk);
extern u32 seccomp_bpf_load(int off);
#else  /* CONFIG_SECCOMP_FILTER */
static inline void put_seccomp_filter(struct task_struct *tsk)
{
	return;
}
static inline void get_seccomp_filter(struct task_struct *tsk)
{
	return;
}
#endif /* CONFIG_SECCOMP_FILTER */
#endif /* __KERNEL__ */
=======
static inline int seccomp_mode(seccomp_t *s)
{
	return 0;
}

#endif /* CONFIG_SECCOMP */

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#endif /* _LINUX_SECCOMP_H */
