#include <linux/kernel.h>
#include <linux/gcd.h>
#include <linux/export.h>

/* Greatest common divisor */
unsigned long gcd(unsigned long a, unsigned long b)
{
	unsigned long r;

	if (a < b)
		swap(a, b);
<<<<<<< HEAD
=======

	if (!b)
		return a;
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	while ((r = a % b) != 0) {
		a = b;
		b = r;
	}
	return b;
}
EXPORT_SYMBOL_GPL(gcd);
