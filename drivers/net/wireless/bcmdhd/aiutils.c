/*
 * Misc utility routines for accessing chip-specific features
 * of the SiliconBackplane-based Broadcom chips.
 *
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
 *
<<<<<<< HEAD
 * $Id: aiutils.c 385510 2013-02-15 21:02:07Z $
=======
 * $Id: aiutils.c 531050 2015-02-02 07:21:19Z $
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 */
#include <bcm_cfg.h>
#include <typedefs.h>
#include <bcmdefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <siutils.h>
#include <hndsoc.h>
#include <sbchipc.h>
#include <pcicfg.h>

#include "siutils_priv.h"

#define BCM47162_DMP() (0)
#define BCM5357_DMP() (0)
#define BCM4707_DMP() (0)
#define remap_coreid(sih, coreid)	(coreid)
#define remap_corerev(sih, corerev)	(corerev)

<<<<<<< HEAD

=======
/* EROM parsing */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

static uint32
get_erom_ent(si_t *sih, uint32 **eromptr, uint32 mask, uint32 match)
{
	uint32 ent;
	uint inv = 0, nom = 0;
<<<<<<< HEAD
=======
	uint32 size = 0;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

	while (TRUE) {
		ent = R_REG(si_osh(sih), *eromptr);
		(*eromptr)++;

		if (mask == 0)
			break;

		if ((ent & ER_VALID) == 0) {
			inv++;
			continue;
		}

		if (ent == (ER_END | ER_VALID))
			break;

		if ((ent & mask) == match)
			break;

<<<<<<< HEAD
=======
		/* escape condition related EROM size if it has invalid values */
		size += sizeof(*eromptr);
		if (size >= ER_SZ_MAX) {
			SI_ERROR(("Failed to find end of EROM marker\n"));
			break;
		}

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		nom++;
	}

	SI_VMSG(("%s: Returning ent 0x%08x\n", __FUNCTION__, ent));
	if (inv + nom) {
		SI_VMSG(("  after %d invalid and %d non-matching entries\n", inv, nom));
	}
	return ent;
}

static uint32
get_asd(si_t *sih, uint32 **eromptr, uint sp, uint ad, uint st, uint32 *addrl, uint32 *addrh,
        uint32 *sizel, uint32 *sizeh)
{
	uint32 asd, sz, szd;

	asd = get_erom_ent(sih, eromptr, ER_VALID, ER_VALID);
	if (((asd & ER_TAG1) != ER_ADD) ||
	    (((asd & AD_SP_MASK) >> AD_SP_SHIFT) != sp) ||
	    ((asd & AD_ST_MASK) != st)) {
<<<<<<< HEAD
		
=======
		/* This is not what we want, "push" it back */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		(*eromptr)--;
		return 0;
	}
	*addrl = asd & AD_ADDR_MASK;
	if (asd & AD_AG32)
		*addrh = get_erom_ent(sih, eromptr, 0, 0);
	else
		*addrh = 0;
	*sizeh = 0;
	sz = asd & AD_SZ_MASK;
	if (sz == AD_SZ_SZD) {
		szd = get_erom_ent(sih, eromptr, 0, 0);
		*sizel = szd & SD_SZ_MASK;
		if (szd & SD_SG32)
			*sizeh = get_erom_ent(sih, eromptr, 0, 0);
	} else
		*sizel = AD_SZ_BASE << (sz >> AD_SZ_SHIFT);

	SI_VMSG(("  SP %d, ad %d: st = %d, 0x%08x_0x%08x @ 0x%08x_0x%08x\n",
	        sp, ad, st, *sizeh, *sizel, *addrh, *addrl));

	return asd;
}

static void
ai_hwfixup(si_info_t *sii)
{
}


<<<<<<< HEAD

=======
/* parse the enumeration rom to identify all cores */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
void
ai_scan(si_t *sih, void *regs, uint devid)
{
	si_info_t *sii = SI_INFO(sih);
<<<<<<< HEAD
=======
	si_cores_info_t *cores_info = (si_cores_info_t *)sii->cores_info;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	chipcregs_t *cc = (chipcregs_t *)regs;
	uint32 erombase, *eromptr, *eromlim;

	erombase = R_REG(sii->osh, &cc->eromptr);

	switch (BUSTYPE(sih->bustype)) {
	case SI_BUS:
		eromptr = (uint32 *)REG_MAP(erombase, SI_CORE_SIZE);
		break;

	case PCI_BUS:
<<<<<<< HEAD
		
		sii->curwrap = (void *)((uintptr)regs + SI_CORE_SIZE);

		
=======
		/* Set wrappers address */
		sii->curwrap = (void *)((uintptr)regs + SI_CORE_SIZE);

		/* Now point the window at the erom */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		OSL_PCI_WRITE_CONFIG(sii->osh, PCI_BAR0_WIN, 4, erombase);
		eromptr = regs;
		break;

	case SPI_BUS:
	case SDIO_BUS:
		eromptr = (uint32 *)(uintptr)erombase;
		break;

	case PCMCIA_BUS:
	default:
		SI_ERROR(("Don't know how to do AXI enumertion on bus %d\n", sih->bustype));
		ASSERT(0);
		return;
	}
	eromlim = eromptr + (ER_REMAPCONTROL / sizeof(uint32));

	SI_VMSG(("ai_scan: regs = 0x%p, erombase = 0x%08x, eromptr = 0x%p, eromlim = 0x%p\n",
	         regs, erombase, eromptr, eromlim));
	while (eromptr < eromlim) {
		uint32 cia, cib, cid, mfg, crev, nmw, nsw, nmp, nsp;
		uint32 mpd, asd, addrl, addrh, sizel, sizeh;
		uint i, j, idx;
		bool br;

		br = FALSE;

<<<<<<< HEAD
		
=======
		/* Grok a component */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		cia = get_erom_ent(sih, &eromptr, ER_TAG, ER_CI);
		if (cia == (ER_END | ER_VALID)) {
			SI_VMSG(("Found END of erom after %d cores\n", sii->numcores));
			ai_hwfixup(sii);
			return;
		}

		cib = get_erom_ent(sih, &eromptr, 0, 0);

		if ((cib & ER_TAG) != ER_CI) {
			SI_ERROR(("CIA not followed by CIB\n"));
			goto error;
		}

		cid = (cia & CIA_CID_MASK) >> CIA_CID_SHIFT;
		mfg = (cia & CIA_MFG_MASK) >> CIA_MFG_SHIFT;
		crev = (cib & CIB_REV_MASK) >> CIB_REV_SHIFT;
		nmw = (cib & CIB_NMW_MASK) >> CIB_NMW_SHIFT;
		nsw = (cib & CIB_NSW_MASK) >> CIB_NSW_SHIFT;
		nmp = (cib & CIB_NMP_MASK) >> CIB_NMP_SHIFT;
		nsp = (cib & CIB_NSP_MASK) >> CIB_NSP_SHIFT;

#ifdef BCMDBG_SI
		SI_VMSG(("Found component 0x%04x/0x%04x rev %d at erom addr 0x%p, with nmw = %d, "
		         "nsw = %d, nmp = %d & nsp = %d\n",
		         mfg, cid, crev, eromptr - 1, nmw, nsw, nmp, nsp));
#else
		BCM_REFERENCE(crev);
#endif

		if (((mfg == MFGID_ARM) && (cid == DEF_AI_COMP)) || (nsp == 0))
			continue;
		if ((nmw + nsw == 0)) {
<<<<<<< HEAD
			
=======
			/* A component which is not a core */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
			if (cid == OOB_ROUTER_CORE_ID) {
				asd = get_asd(sih, &eromptr, 0, 0, AD_ST_SLAVE,
					&addrl, &addrh, &sizel, &sizeh);
				if (asd != 0) {
					sii->oob_router = addrl;
				}
			}
			if (cid != GMAC_COMMON_4706_CORE_ID && cid != NS_CCB_CORE_ID)
				continue;
		}

		idx = sii->numcores;

<<<<<<< HEAD
		sii->cia[idx] = cia;
		sii->cib[idx] = cib;
		sii->coreid[idx] = remap_coreid(sih, cid);
=======
		cores_info->cia[idx] = cia;
		cores_info->cib[idx] = cib;
		cores_info->coreid[idx] = remap_coreid(sih, cid);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

		for (i = 0; i < nmp; i++) {
			mpd = get_erom_ent(sih, &eromptr, ER_VALID, ER_VALID);
			if ((mpd & ER_TAG) != ER_MP) {
				SI_ERROR(("Not enough MP entries for component 0x%x\n", cid));
				goto error;
			}
			SI_VMSG(("  Master port %d, mp: %d id: %d\n", i,
			         (mpd & MPD_MP_MASK) >> MPD_MP_SHIFT,
			         (mpd & MPD_MUI_MASK) >> MPD_MUI_SHIFT));
		}

<<<<<<< HEAD
		
		asd = get_asd(sih, &eromptr, 0, 0, AD_ST_SLAVE, &addrl, &addrh, &sizel, &sizeh);
		if (asd == 0) {
			do {
			
=======
		/* First Slave Address Descriptor should be port 0:
		 * the main register space for the core
		 */
		asd = get_asd(sih, &eromptr, 0, 0, AD_ST_SLAVE, &addrl, &addrh, &sizel, &sizeh);
		if (asd == 0) {
			do {
			/* Try again to see if it is a bridge */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
			asd = get_asd(sih, &eromptr, 0, 0, AD_ST_BRIDGE, &addrl, &addrh,
			              &sizel, &sizeh);
			if (asd != 0)
				br = TRUE;
			else {
					if (br == TRUE) {
						break;
					}
					else if ((addrh != 0) || (sizeh != 0) ||
						(sizel != SI_CORE_SIZE)) {
						SI_ERROR(("addrh = 0x%x\t sizeh = 0x%x\t size1 ="
							"0x%x\n", addrh, sizeh, sizel));
						SI_ERROR(("First Slave ASD for"
							"core 0x%04x malformed "
							"(0x%08x)\n", cid, asd));
						goto error;
					}
				}
			} while (1);
		}
<<<<<<< HEAD
		sii->coresba[idx] = addrl;
		sii->coresba_size[idx] = sizel;
		
=======
		cores_info->coresba[idx] = addrl;
		cores_info->coresba_size[idx] = sizel;
		/* Get any more ASDs in port 0 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		j = 1;
		do {
			asd = get_asd(sih, &eromptr, 0, j, AD_ST_SLAVE, &addrl, &addrh,
			              &sizel, &sizeh);
			if ((asd != 0) && (j == 1) && (sizel == SI_CORE_SIZE)) {
<<<<<<< HEAD
				sii->coresba2[idx] = addrl;
				sii->coresba2_size[idx] = sizel;
=======
				cores_info->coresba2[idx] = addrl;
				cores_info->coresba2_size[idx] = sizel;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
			}
			j++;
		} while (asd != 0);

<<<<<<< HEAD
		
=======
		/* Go through the ASDs for other slave ports */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		for (i = 1; i < nsp; i++) {
			j = 0;
			do {
				asd = get_asd(sih, &eromptr, i, j, AD_ST_SLAVE, &addrl, &addrh,
				              &sizel, &sizeh);

				if (asd == 0)
					break;
				j++;
			} while (1);
			if (j == 0) {
				SI_ERROR((" SP %d has no address descriptors\n", i));
				goto error;
			}
		}

<<<<<<< HEAD
		
=======
		/* Now get master wrappers */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		for (i = 0; i < nmw; i++) {
			asd = get_asd(sih, &eromptr, i, 0, AD_ST_MWRAP, &addrl, &addrh,
			              &sizel, &sizeh);
			if (asd == 0) {
				SI_ERROR(("Missing descriptor for MW %d\n", i));
				goto error;
			}
			if ((sizeh != 0) || (sizel != SI_CORE_SIZE)) {
				SI_ERROR(("Master wrapper %d is not 4KB\n", i));
				goto error;
			}
			if (i == 0)
<<<<<<< HEAD
				sii->wrapba[idx] = addrl;
		}

		
=======
				cores_info->wrapba[idx] = addrl;
		}

		/* And finally slave wrappers */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		for (i = 0; i < nsw; i++) {
			uint fwp = (nsp == 1) ? 0 : 1;
			asd = get_asd(sih, &eromptr, fwp + i, 0, AD_ST_SWRAP, &addrl, &addrh,
			              &sizel, &sizeh);
			if (asd == 0) {
				SI_ERROR(("Missing descriptor for SW %d\n", i));
				goto error;
			}
			if ((sizeh != 0) || (sizel != SI_CORE_SIZE)) {
				SI_ERROR(("Slave wrapper %d is not 4KB\n", i));
				goto error;
			}
			if ((nmw == 0) && (i == 0))
<<<<<<< HEAD
				sii->wrapba[idx] = addrl;
		}


		
		if (br)
			continue;

		
=======
				cores_info->wrapba[idx] = addrl;
		}


		/* Don't record bridges */
		if (br)
			continue;

		/* Done with core */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		sii->numcores++;
	}

	SI_ERROR(("Reached end of erom without finding END"));

error:
	sii->numcores = 0;
	return;
}

<<<<<<< HEAD

=======
/* This function changes the logical "focus" to the indicated core.
 * Return the current core's virtual address.
 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
void *
ai_setcoreidx(si_t *sih, uint coreidx)
{
	si_info_t *sii = SI_INFO(sih);
<<<<<<< HEAD
=======
	si_cores_info_t *cores_info = (si_cores_info_t *)sii->cores_info;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	uint32 addr, wrap;
	void *regs;

	if (coreidx >= MIN(sii->numcores, SI_MAXCORES))
		return (NULL);

<<<<<<< HEAD
	addr = sii->coresba[coreidx];
	wrap = sii->wrapba[coreidx];

	
=======
	addr = cores_info->coresba[coreidx];
	wrap = cores_info->wrapba[coreidx];

	/*
	 * If the user has provided an interrupt mask enabled function,
	 * then assert interrupts are disabled before switching the core.
	 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	ASSERT((sii->intrsenabled_fn == NULL) || !(*(sii)->intrsenabled_fn)((sii)->intr_arg));

	switch (BUSTYPE(sih->bustype)) {
	case SI_BUS:
<<<<<<< HEAD
		
		if (!sii->regs[coreidx]) {
			sii->regs[coreidx] = REG_MAP(addr, SI_CORE_SIZE);
			ASSERT(GOODREGS(sii->regs[coreidx]));
		}
		sii->curmap = regs = sii->regs[coreidx];
		if (!sii->wrappers[coreidx] && (wrap != 0)) {
			sii->wrappers[coreidx] = REG_MAP(wrap, SI_CORE_SIZE);
			ASSERT(GOODREGS(sii->wrappers[coreidx]));
		}
		sii->curwrap = sii->wrappers[coreidx];
		break;

=======
		/* map new one */
		if (!cores_info->regs[coreidx]) {
			cores_info->regs[coreidx] = REG_MAP(addr, SI_CORE_SIZE);
			ASSERT(GOODREGS(cores_info->regs[coreidx]));
		}
		sii->curmap = regs = cores_info->regs[coreidx];
		if (!cores_info->wrappers[coreidx] && (wrap != 0)) {
			cores_info->wrappers[coreidx] = REG_MAP(wrap, SI_CORE_SIZE);
			ASSERT(GOODREGS(cores_info->wrappers[coreidx]));
		}
		sii->curwrap = cores_info->wrappers[coreidx];
		break;

	case PCI_BUS:
		/* point bar0 window */
		OSL_PCI_WRITE_CONFIG(sii->osh, PCI_BAR0_WIN, 4, addr);
		regs = sii->curmap;
		/* point bar0 2nd 4KB window to the primary wrapper */
		if (PCIE_GEN2(sii))
			OSL_PCI_WRITE_CONFIG(sii->osh, PCIE2_BAR0_WIN2, 4, wrap);
		else
			OSL_PCI_WRITE_CONFIG(sii->osh, PCI_BAR0_WIN2, 4, wrap);
		break;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

	case SPI_BUS:
	case SDIO_BUS:
		sii->curmap = regs = (void *)((uintptr)addr);
		sii->curwrap = (void *)((uintptr)wrap);
		break;

	case PCMCIA_BUS:
	default:
		ASSERT(0);
		regs = NULL;
		break;
	}

	sii->curmap = regs;
	sii->curidx = coreidx;

	return regs;
}

<<<<<<< HEAD
=======

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
void
ai_coreaddrspaceX(si_t *sih, uint asidx, uint32 *addr, uint32 *size)
{
	si_info_t *sii = SI_INFO(sih);
<<<<<<< HEAD
=======
	si_cores_info_t *cores_info = (si_cores_info_t *)sii->cores_info;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	chipcregs_t *cc = NULL;
	uint32 erombase, *eromptr, *eromlim;
	uint i, j, cidx;
	uint32 cia, cib, nmp, nsp;
	uint32 asd, addrl, addrh, sizel, sizeh;

	for (i = 0; i < sii->numcores; i++) {
<<<<<<< HEAD
		if (sii->coreid[i] == CC_CORE_ID) {
			cc = (chipcregs_t *)sii->regs[i];
=======
		if (cores_info->coreid[i] == CC_CORE_ID) {
			cc = (chipcregs_t *)cores_info->regs[i];
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
			break;
		}
	}
	if (cc == NULL)
		goto error;

	erombase = R_REG(sii->osh, &cc->eromptr);
	eromptr = (uint32 *)REG_MAP(erombase, SI_CORE_SIZE);
	eromlim = eromptr + (ER_REMAPCONTROL / sizeof(uint32));

	cidx = sii->curidx;
<<<<<<< HEAD
	cia = sii->cia[cidx];
	cib = sii->cib[cidx];
=======
	cia = cores_info->cia[cidx];
	cib = cores_info->cib[cidx];
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

	nmp = (cib & CIB_NMP_MASK) >> CIB_NMP_SHIFT;
	nsp = (cib & CIB_NSP_MASK) >> CIB_NSP_SHIFT;

<<<<<<< HEAD
	
=======
	/* scan for cores */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	while (eromptr < eromlim) {
		if ((get_erom_ent(sih, &eromptr, ER_TAG, ER_CI) == cia) &&
			(get_erom_ent(sih, &eromptr, 0, 0) == cib)) {
			break;
		}
	}

<<<<<<< HEAD
	
	for (i = 0; i < nmp; i++)
		get_erom_ent(sih, &eromptr, ER_VALID, ER_VALID);

	
	asd = get_asd(sih, &eromptr, 0, 0, AD_ST_SLAVE, &addrl, &addrh, &sizel, &sizeh);
	if (asd == 0) {
		
=======
	/* skip master ports */
	for (i = 0; i < nmp; i++)
		get_erom_ent(sih, &eromptr, ER_VALID, ER_VALID);

	/* Skip ASDs in port 0 */
	asd = get_asd(sih, &eromptr, 0, 0, AD_ST_SLAVE, &addrl, &addrh, &sizel, &sizeh);
	if (asd == 0) {
		/* Try again to see if it is a bridge */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		asd = get_asd(sih, &eromptr, 0, 0, AD_ST_BRIDGE, &addrl, &addrh,
		              &sizel, &sizeh);
	}

	j = 1;
	do {
		asd = get_asd(sih, &eromptr, 0, j, AD_ST_SLAVE, &addrl, &addrh,
		              &sizel, &sizeh);
		j++;
	} while (asd != 0);

<<<<<<< HEAD
	
=======
	/* Go through the ASDs for other slave ports */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	for (i = 1; i < nsp; i++) {
		j = 0;
		do {
			asd = get_asd(sih, &eromptr, i, j, AD_ST_SLAVE, &addrl, &addrh,
				&sizel, &sizeh);
			if (asd == 0)
				break;

			if (!asidx--) {
				*addr = addrl;
				*size = sizel;
				return;
			}
			j++;
		} while (1);

		if (j == 0) {
			SI_ERROR((" SP %d has no address descriptors\n", i));
			break;
		}
	}

error:
	*size = 0;
	return;
}

<<<<<<< HEAD

=======
/* Return the number of address spaces in current core */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
int
ai_numaddrspaces(si_t *sih)
{
	return 2;
}

<<<<<<< HEAD

uint32
ai_addrspace(si_t *sih, uint asidx)
{
	si_info_t *sii;
	uint cidx;

	sii = SI_INFO(sih);
	cidx = sii->curidx;

	if (asidx == 0)
		return sii->coresba[cidx];
	else if (asidx == 1)
		return sii->coresba2[cidx];
=======
/* Return the address of the nth address space in the current core */
uint32
ai_addrspace(si_t *sih, uint asidx)
{
	si_info_t *sii = SI_INFO(sih);
	si_cores_info_t *cores_info = (si_cores_info_t *)sii->cores_info;
	uint cidx;

	cidx = sii->curidx;

	if (asidx == 0)
		return cores_info->coresba[cidx];
	else if (asidx == 1)
		return cores_info->coresba2[cidx];
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	else {
		SI_ERROR(("%s: Need to parse the erom again to find addr space %d\n",
		          __FUNCTION__, asidx));
		return 0;
	}
}

<<<<<<< HEAD

uint32
ai_addrspacesize(si_t *sih, uint asidx)
{
	si_info_t *sii;
	uint cidx;

	sii = SI_INFO(sih);
	cidx = sii->curidx;

	if (asidx == 0)
		return sii->coresba_size[cidx];
	else if (asidx == 1)
		return sii->coresba2_size[cidx];
=======
/* Return the size of the nth address space in the current core */
uint32
ai_addrspacesize(si_t *sih, uint asidx)
{
	si_info_t *sii = SI_INFO(sih);
	si_cores_info_t *cores_info = (si_cores_info_t *)sii->cores_info;
	uint cidx;

	cidx = sii->curidx;

	if (asidx == 0)
		return cores_info->coresba_size[cidx];
	else if (asidx == 1)
		return cores_info->coresba2_size[cidx];
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	else {
		SI_ERROR(("%s: Need to parse the erom again to find addr space %d\n",
		          __FUNCTION__, asidx));
		return 0;
	}
}

uint
ai_flag(si_t *sih)
{
<<<<<<< HEAD
	si_info_t *sii;
	aidmp_t *ai;

	sii = SI_INFO(sih);
=======
	si_info_t *sii = SI_INFO(sih);
	aidmp_t *ai;

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	if (BCM47162_DMP()) {
		SI_ERROR(("%s: Attempting to read MIPS DMP registers on 47162a0", __FUNCTION__));
		return sii->curidx;
	}
	if (BCM5357_DMP()) {
		SI_ERROR(("%s: Attempting to read USB20H DMP registers on 5357b0\n", __FUNCTION__));
		return sii->curidx;
	}
	if (BCM4707_DMP()) {
		SI_ERROR(("%s: Attempting to read CHIPCOMMONB DMP registers on 4707\n",
			__FUNCTION__));
		return sii->curidx;
	}
	ai = sii->curwrap;

	return (R_REG(sii->osh, &ai->oobselouta30) & 0x1f);
}

uint
ai_flag_alt(si_t *sih)
{
<<<<<<< HEAD
	si_info_t *sii;
	aidmp_t *ai;

	sii = SI_INFO(sih);
=======
	si_info_t *sii = SI_INFO(sih);
	aidmp_t *ai;

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	if (BCM47162_DMP()) {
		SI_ERROR(("%s: Attempting to read MIPS DMP registers on 47162a0", __FUNCTION__));
		return sii->curidx;
	}
	if (BCM5357_DMP()) {
		SI_ERROR(("%s: Attempting to read USB20H DMP registers on 5357b0\n", __FUNCTION__));
		return sii->curidx;
	}
	if (BCM4707_DMP()) {
		SI_ERROR(("%s: Attempting to read CHIPCOMMONB DMP registers on 4707\n",
			__FUNCTION__));
		return sii->curidx;
	}
	ai = sii->curwrap;

	return ((R_REG(sii->osh, &ai->oobselouta30) >> AI_OOBSEL_1_SHIFT) & AI_OOBSEL_MASK);
}

void
ai_setint(si_t *sih, int siflag)
{
}

uint
ai_wrap_reg(si_t *sih, uint32 offset, uint32 mask, uint32 val)
{
	si_info_t *sii = SI_INFO(sih);
	uint32 *map = (uint32 *) sii->curwrap;

	if (mask || val) {
		uint32 w = R_REG(sii->osh, map+(offset/4));
		w &= ~mask;
		w |= val;
<<<<<<< HEAD
		W_REG(sii->osh, map+(offset/4), val);
=======
		W_REG(sii->osh, map+(offset/4), w);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	}

	return (R_REG(sii->osh, map+(offset/4)));
}

uint
ai_corevendor(si_t *sih)
{
<<<<<<< HEAD
	si_info_t *sii;
	uint32 cia;

	sii = SI_INFO(sih);
	cia = sii->cia[sii->curidx];
=======
	si_info_t *sii = SI_INFO(sih);
	si_cores_info_t *cores_info = (si_cores_info_t *)sii->cores_info;
	uint32 cia;

	cia = cores_info->cia[sii->curidx];
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	return ((cia & CIA_MFG_MASK) >> CIA_MFG_SHIFT);
}

uint
ai_corerev(si_t *sih)
{
<<<<<<< HEAD
	si_info_t *sii;
	uint32 cib;

	sii = SI_INFO(sih);
	cib = sii->cib[sii->curidx];
=======
	si_info_t *sii = SI_INFO(sih);
	si_cores_info_t *cores_info = (si_cores_info_t *)sii->cores_info;
	uint32 cib;


	cib = cores_info->cib[sii->curidx];
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	return remap_corerev(sih, (cib & CIB_REV_MASK) >> CIB_REV_SHIFT);
}

bool
ai_iscoreup(si_t *sih)
{
<<<<<<< HEAD
	si_info_t *sii;
	aidmp_t *ai;

	sii = SI_INFO(sih);
=======
	si_info_t *sii = SI_INFO(sih);
	aidmp_t *ai;

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	ai = sii->curwrap;

	return (((R_REG(sii->osh, &ai->ioctrl) & (SICF_FGC | SICF_CLOCK_EN)) == SICF_CLOCK_EN) &&
	        ((R_REG(sii->osh, &ai->resetctrl) & AIRC_RESET) == 0));
}

<<<<<<< HEAD

=======
/*
 * Switch to 'coreidx', issue a single arbitrary 32bit register mask&set operation,
 * switch back to the original core, and return the new value.
 *
 * When using the silicon backplane, no fiddling with interrupts or core switches is needed.
 *
 * Also, when using pci/pcie, we can optimize away the core switching for pci registers
 * and (on newer pci cores) chipcommon registers.
 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
uint
ai_corereg(si_t *sih, uint coreidx, uint regoff, uint mask, uint val)
{
	uint origidx = 0;
	uint32 *r = NULL;
	uint w;
	uint intr_val = 0;
	bool fast = FALSE;
<<<<<<< HEAD
	si_info_t *sii;

	sii = SI_INFO(sih);
=======
	si_info_t *sii = SI_INFO(sih);
	si_cores_info_t *cores_info = (si_cores_info_t *)sii->cores_info;

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

	ASSERT(GOODIDX(coreidx));
	ASSERT(regoff < SI_CORE_SIZE);
	ASSERT((val & ~mask) == 0);

	if (coreidx >= SI_MAXCORES)
		return 0;

	if (BUSTYPE(sih->bustype) == SI_BUS) {
<<<<<<< HEAD
		
		fast = TRUE;
		
		if (!sii->regs[coreidx]) {
			sii->regs[coreidx] = REG_MAP(sii->coresba[coreidx],
			                            SI_CORE_SIZE);
			ASSERT(GOODREGS(sii->regs[coreidx]));
		}
		r = (uint32 *)((uchar *)sii->regs[coreidx] + regoff);
	} else if (BUSTYPE(sih->bustype) == PCI_BUS) {
		

		if ((sii->coreid[coreidx] == CC_CORE_ID) && SI_FAST(sii)) {
			
=======
		/* If internal bus, we can always get at everything */
		fast = TRUE;
		/* map if does not exist */
		if (!cores_info->regs[coreidx]) {
			cores_info->regs[coreidx] = REG_MAP(cores_info->coresba[coreidx],
			                            SI_CORE_SIZE);
			ASSERT(GOODREGS(cores_info->regs[coreidx]));
		}
		r = (uint32 *)((uchar *)cores_info->regs[coreidx] + regoff);
	} else if (BUSTYPE(sih->bustype) == PCI_BUS) {
		/* If pci/pcie, we can get at pci/pcie regs and on newer cores to chipc */

		if ((cores_info->coreid[coreidx] == CC_CORE_ID) && SI_FAST(sii)) {
			/* Chipc registers are mapped at 12KB */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

			fast = TRUE;
			r = (uint32 *)((char *)sii->curmap + PCI_16KB0_CCREGS_OFFSET + regoff);
		} else if (sii->pub.buscoreidx == coreidx) {
<<<<<<< HEAD
			
=======
			/* pci registers are at either in the last 2KB of an 8KB window
			 * or, in pcie and pci rev 13 at 8KB
			 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
			fast = TRUE;
			if (SI_FAST(sii))
				r = (uint32 *)((char *)sii->curmap +
				               PCI_16KB0_PCIREGS_OFFSET + regoff);
			else
				r = (uint32 *)((char *)sii->curmap +
				               ((regoff >= SBCONFIGOFF) ?
				                PCI_BAR0_PCISBR_OFFSET : PCI_BAR0_PCIREGS_OFFSET) +
				               regoff);
		}
	}

	if (!fast) {
		INTR_OFF(sii, intr_val);

<<<<<<< HEAD
		
		origidx = si_coreidx(&sii->pub);

		
=======
		/* save current core index */
		origidx = si_coreidx(&sii->pub);

		/* switch core */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		r = (uint32*) ((uchar*) ai_setcoreidx(&sii->pub, coreidx) + regoff);
	}
	ASSERT(r != NULL);

<<<<<<< HEAD
	
=======
	/* mask and set */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	if (mask || val) {
		w = (R_REG(sii->osh, r) & ~mask) | val;
		W_REG(sii->osh, r, w);
	}

<<<<<<< HEAD
	
	w = R_REG(sii->osh, r);

	if (!fast) {
		
=======
	/* readback */
	w = R_REG(sii->osh, r);

	if (!fast) {
		/* restore core index */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		if (origidx != coreidx)
			ai_setcoreidx(&sii->pub, origidx);

		INTR_RESTORE(sii, intr_val);
	}

	return (w);
}

<<<<<<< HEAD
void
ai_core_disable(si_t *sih, uint32 bits)
{
	si_info_t *sii;
=======
/*
 * If there is no need for fiddling with interrupts or core switches (typically silicon
 * back plane registers, pci registers and chipcommon registers), this function
 * returns the register offset on this core to a mapped address. This address can
 * be used for W_REG/R_REG directly.
 *
 * For accessing registers that would need a core switch, this function will return
 * NULL.
 */
uint32 *
ai_corereg_addr(si_t *sih, uint coreidx, uint regoff)
{
	uint32 *r = NULL;
	bool fast = FALSE;
	si_info_t *sii = SI_INFO(sih);
	si_cores_info_t *cores_info = (si_cores_info_t *)sii->cores_info;


	ASSERT(GOODIDX(coreidx));
	ASSERT(regoff < SI_CORE_SIZE);

	if (coreidx >= SI_MAXCORES)
		return 0;

	if (BUSTYPE(sih->bustype) == SI_BUS) {
		/* If internal bus, we can always get at everything */
		fast = TRUE;
		/* map if does not exist */
		if (!cores_info->regs[coreidx]) {
			cores_info->regs[coreidx] = REG_MAP(cores_info->coresba[coreidx],
			                            SI_CORE_SIZE);
			ASSERT(GOODREGS(cores_info->regs[coreidx]));
		}
		r = (uint32 *)((uchar *)cores_info->regs[coreidx] + regoff);
	} else if (BUSTYPE(sih->bustype) == PCI_BUS) {
		/* If pci/pcie, we can get at pci/pcie regs and on newer cores to chipc */

		if ((cores_info->coreid[coreidx] == CC_CORE_ID) && SI_FAST(sii)) {
			/* Chipc registers are mapped at 12KB */

			fast = TRUE;
			r = (uint32 *)((char *)sii->curmap + PCI_16KB0_CCREGS_OFFSET + regoff);
		} else if (sii->pub.buscoreidx == coreidx) {
			/* pci registers are at either in the last 2KB of an 8KB window
			 * or, in pcie and pci rev 13 at 8KB
			 */
			fast = TRUE;
			if (SI_FAST(sii))
				r = (uint32 *)((char *)sii->curmap +
				               PCI_16KB0_PCIREGS_OFFSET + regoff);
			else
				r = (uint32 *)((char *)sii->curmap +
				               ((regoff >= SBCONFIGOFF) ?
				                PCI_BAR0_PCISBR_OFFSET : PCI_BAR0_PCIREGS_OFFSET) +
				               regoff);
		}
	}

	if (!fast)
		return 0;

	return (r);
}

void
ai_core_disable(si_t *sih, uint32 bits)
{
	si_info_t *sii = SI_INFO(sih);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	volatile uint32 dummy;
	uint32 status;
	aidmp_t *ai;

<<<<<<< HEAD
	sii = SI_INFO(sih);
=======
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

	ASSERT(GOODREGS(sii->curwrap));
	ai = sii->curwrap;

<<<<<<< HEAD
	
	if (R_REG(sii->osh, &ai->resetctrl) & AIRC_RESET)
		return;

	
	SPINWAIT(((status = R_REG(sii->osh, &ai->resetstatus)) != 0), 300);

	
	if (status != 0) {
		
		
		SPINWAIT(((status = R_REG(sii->osh, &ai->resetstatus)) != 0), 10000);
		
		
=======
	/* if core is already in reset, just return */
	if (R_REG(sii->osh, &ai->resetctrl) & AIRC_RESET)
		return;

	/* ensure there are no pending backplane operations */
	SPINWAIT(((status = R_REG(sii->osh, &ai->resetstatus)) != 0), 300);

	/* if pending backplane ops still, try waiting longer */
	if (status != 0) {
		/* 300usecs was sufficient to allow backplane ops to clear for big hammer */
		/* during driver load we may need more time */
		SPINWAIT(((status = R_REG(sii->osh, &ai->resetstatus)) != 0), 10000);
		/* if still pending ops, continue on and try disable anyway */
		/* this is in big hammer path, so don't call wl_reinit in this case... */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	}

	W_REG(sii->osh, &ai->resetctrl, AIRC_RESET);
	dummy = R_REG(sii->osh, &ai->resetctrl);
	BCM_REFERENCE(dummy);
	OSL_DELAY(1);

	W_REG(sii->osh, &ai->ioctrl, bits);
	dummy = R_REG(sii->osh, &ai->ioctrl);
	BCM_REFERENCE(dummy);
	OSL_DELAY(10);
}

<<<<<<< HEAD

void
ai_core_reset(si_t *sih, uint32 bits, uint32 resetbits)
{
	si_info_t *sii;
	aidmp_t *ai;
	volatile uint32 dummy;

	sii = SI_INFO(sih);
	ASSERT(GOODREGS(sii->curwrap));
	ai = sii->curwrap;

	
	ai_core_disable(sih, (bits | resetbits));

	
	W_REG(sii->osh, &ai->ioctrl, (bits | SICF_FGC | SICF_CLOCK_EN));
	dummy = R_REG(sii->osh, &ai->ioctrl);
	BCM_REFERENCE(dummy);

	W_REG(sii->osh, &ai->resetctrl, 0);
	dummy = R_REG(sii->osh, &ai->resetctrl);
	BCM_REFERENCE(dummy);
	OSL_DELAY(1);
=======
/* reset and re-enable a core
 * inputs:
 * bits - core specific bits that are set during and after reset sequence
 * resetbits - core specific bits that are set only during reset sequence
 */
void
ai_core_reset(si_t *sih, uint32 bits, uint32 resetbits)
{
	si_info_t *sii = SI_INFO(sih);
	aidmp_t *ai;
	volatile uint32 dummy;
	uint loop_counter = 10;

	ASSERT(GOODREGS(sii->curwrap));
	ai = sii->curwrap;

	/* ensure there are no pending backplane operations */
	SPINWAIT(((dummy = R_REG(sii->osh, &ai->resetstatus)) != 0), 300);


	/* put core into reset state */
	W_REG(sii->osh, &ai->resetctrl, AIRC_RESET);
	OSL_DELAY(10);

	/* ensure there are no pending backplane operations */
	SPINWAIT((R_REG(sii->osh, &ai->resetstatus) != 0), 300);

	W_REG(sii->osh, &ai->ioctrl, (bits | resetbits | SICF_FGC | SICF_CLOCK_EN));
	dummy = R_REG(sii->osh, &ai->ioctrl);
	BCM_REFERENCE(dummy);

	/* ensure there are no pending backplane operations */
	SPINWAIT(((dummy = R_REG(sii->osh, &ai->resetstatus)) != 0), 300);


	while (R_REG(sii->osh, &ai->resetctrl) != 0 && --loop_counter != 0) {
		/* ensure there are no pending backplane operations */
		SPINWAIT(((dummy = R_REG(sii->osh, &ai->resetstatus)) != 0), 300);


		/* take core out of reset */
		W_REG(sii->osh, &ai->resetctrl, 0);

		/* ensure there are no pending backplane operations */
		SPINWAIT((R_REG(sii->osh, &ai->resetstatus) != 0), 300);
	}

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

	W_REG(sii->osh, &ai->ioctrl, (bits | SICF_CLOCK_EN));
	dummy = R_REG(sii->osh, &ai->ioctrl);
	BCM_REFERENCE(dummy);
	OSL_DELAY(1);
}

void
ai_core_cflags_wo(si_t *sih, uint32 mask, uint32 val)
{
<<<<<<< HEAD
	si_info_t *sii;
	aidmp_t *ai;
	uint32 w;

	sii = SI_INFO(sih);
=======
	si_info_t *sii = SI_INFO(sih);
	aidmp_t *ai;
	uint32 w;

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

	if (BCM47162_DMP()) {
		SI_ERROR(("%s: Accessing MIPS DMP register (ioctrl) on 47162a0",
		          __FUNCTION__));
		return;
	}
	if (BCM5357_DMP()) {
		SI_ERROR(("%s: Accessing USB20H DMP register (ioctrl) on 5357\n",
		          __FUNCTION__));
		return;
	}
	if (BCM4707_DMP()) {
		SI_ERROR(("%s: Accessing CHIPCOMMONB DMP register (ioctrl) on 4707\n",
			__FUNCTION__));
		return;
	}

	ASSERT(GOODREGS(sii->curwrap));
	ai = sii->curwrap;

	ASSERT((val & ~mask) == 0);

	if (mask || val) {
		w = ((R_REG(sii->osh, &ai->ioctrl) & ~mask) | val);
		W_REG(sii->osh, &ai->ioctrl, w);
	}
}

uint32
ai_core_cflags(si_t *sih, uint32 mask, uint32 val)
{
<<<<<<< HEAD
	si_info_t *sii;
	aidmp_t *ai;
	uint32 w;

	sii = SI_INFO(sih);
=======
	si_info_t *sii = SI_INFO(sih);
	aidmp_t *ai;
	uint32 w;

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	if (BCM47162_DMP()) {
		SI_ERROR(("%s: Accessing MIPS DMP register (ioctrl) on 47162a0",
		          __FUNCTION__));
		return 0;
	}
	if (BCM5357_DMP()) {
		SI_ERROR(("%s: Accessing USB20H DMP register (ioctrl) on 5357\n",
		          __FUNCTION__));
		return 0;
	}
	if (BCM4707_DMP()) {
		SI_ERROR(("%s: Accessing CHIPCOMMONB DMP register (ioctrl) on 4707\n",
			__FUNCTION__));
		return 0;
	}

	ASSERT(GOODREGS(sii->curwrap));
	ai = sii->curwrap;

	ASSERT((val & ~mask) == 0);

	if (mask || val) {
		w = ((R_REG(sii->osh, &ai->ioctrl) & ~mask) | val);
		W_REG(sii->osh, &ai->ioctrl, w);
	}

	return R_REG(sii->osh, &ai->ioctrl);
}

uint32
ai_core_sflags(si_t *sih, uint32 mask, uint32 val)
{
<<<<<<< HEAD
	si_info_t *sii;
	aidmp_t *ai;
	uint32 w;

	sii = SI_INFO(sih);
=======
	si_info_t *sii = SI_INFO(sih);
	aidmp_t *ai;
	uint32 w;

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	if (BCM47162_DMP()) {
		SI_ERROR(("%s: Accessing MIPS DMP register (iostatus) on 47162a0",
		          __FUNCTION__));
		return 0;
	}
	if (BCM5357_DMP()) {
		SI_ERROR(("%s: Accessing USB20H DMP register (iostatus) on 5357\n",
		          __FUNCTION__));
		return 0;
	}
	if (BCM4707_DMP()) {
		SI_ERROR(("%s: Accessing CHIPCOMMONB DMP register (ioctrl) on 4707\n",
			__FUNCTION__));
		return 0;
	}

	ASSERT(GOODREGS(sii->curwrap));
	ai = sii->curwrap;

	ASSERT((val & ~mask) == 0);
	ASSERT((mask & ~SISF_CORE_BITS) == 0);

	if (mask || val) {
		w = ((R_REG(sii->osh, &ai->iostatus) & ~mask) | val);
		W_REG(sii->osh, &ai->iostatus, w);
	}

	return R_REG(sii->osh, &ai->iostatus);
}
