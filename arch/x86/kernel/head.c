#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/memblock.h>

#include <asm/setup.h>
#include <asm/bios_ebda.h>

<<<<<<< HEAD
#define BIOS_LOWMEM_KILOBYTES 0x413

=======
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
/*
 * The BIOS places the EBDA/XBDA at the top of conventional
 * memory, and usually decreases the reported amount of
 * conventional memory (int 0x12) too. This also contains a
 * workaround for Dell systems that neglect to reserve EBDA.
 * The same workaround also avoids a problem with the AMD768MPX
 * chipset: reserve a page before VGA to prevent PCI prefetch
 * into it (errata #56). Usually the page is reserved anyways,
 * unless you have no PS/2 mouse plugged in.
<<<<<<< HEAD
 */
=======
 *
 * This functions is deliberately very conservative.  Losing
 * memory in the bottom megabyte is rarely a problem, as long
 * as we have enough memory to install the trampoline.  Using
 * memory that is in use by the BIOS or by some DMA device
 * the BIOS didn't shut down *is* a big problem.
 */

#define BIOS_LOWMEM_KILOBYTES	0x413
#define LOWMEM_CAP		0x9f000U	/* Absolute maximum */
#define INSANE_CUTOFF		0x20000U	/* Less than this = insane */

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
void __init reserve_ebda_region(void)
{
	unsigned int lowmem, ebda_addr;

<<<<<<< HEAD
	/* To determine the position of the EBDA and the */
	/* end of conventional memory, we need to look at */
	/* the BIOS data area. In a paravirtual environment */
	/* that area is absent. We'll just have to assume */
	/* that the paravirt case can handle memory setup */
	/* correctly, without our help. */
=======
	/*
	 * To determine the position of the EBDA and the
	 * end of conventional memory, we need to look at
	 * the BIOS data area. In a paravirtual environment
	 * that area is absent. We'll just have to assume
	 * that the paravirt case can handle memory setup
	 * correctly, without our help.
	 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	if (paravirt_enabled())
		return;

	/* end of low (conventional) memory */
	lowmem = *(unsigned short *)__va(BIOS_LOWMEM_KILOBYTES);
	lowmem <<= 10;

	/* start of EBDA area */
	ebda_addr = get_bios_ebda();

<<<<<<< HEAD
	/* Fixup: bios puts an EBDA in the top 64K segment */
	/* of conventional memory, but does not adjust lowmem. */
	if ((lowmem - ebda_addr) <= 0x10000)
		lowmem = ebda_addr;

	/* Fixup: bios does not report an EBDA at all. */
	/* Some old Dells seem to need 4k anyhow (bugzilla 2990) */
	if ((ebda_addr == 0) && (lowmem >= 0x9f000))
		lowmem = 0x9f000;

	/* Paranoia: should never happen, but... */
	if ((lowmem == 0) || (lowmem >= 0x100000))
		lowmem = 0x9f000;
=======
	/*
	 * Note: some old Dells seem to need 4k EBDA without
	 * reporting so, so just consider the memory above 0x9f000
	 * to be off limits (bugzilla 2990).
	 */

	/* If the EBDA address is below 128K, assume it is bogus */
	if (ebda_addr < INSANE_CUTOFF)
		ebda_addr = LOWMEM_CAP;

	/* If lowmem is less than 128K, assume it is bogus */
	if (lowmem < INSANE_CUTOFF)
		lowmem = LOWMEM_CAP;

	/* Use the lower of the lowmem and EBDA markers as the cutoff */
	lowmem = min(lowmem, ebda_addr);
	lowmem = min(lowmem, LOWMEM_CAP); /* Absolute cap */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

	/* reserve all memory between lowmem and the 1MB mark */
	memblock_reserve(lowmem, 0x100000 - lowmem);
}
