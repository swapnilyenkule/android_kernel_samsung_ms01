/*
 * SDIO access interface for drivers - linux specific (pci only)
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
 * $Id: bcmsdh_linux.c 414953 2013-07-26 17:36:27Z $
=======
 * $Id: bcmsdh_linux.c 461443 2014-03-12 02:40:59Z $
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 */

/**
 * @file bcmsdh_linux.c
 */

#define __UNDEF_NO_VERSION__

#include <typedefs.h>
#include <linuxver.h>
<<<<<<< HEAD

=======
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#include <linux/pci.h>
#include <linux/completion.h>

#include <osl.h>
#include <pcicfg.h>
#include <bcmdefs.h>
#include <bcmdevs.h>
<<<<<<< HEAD

#if defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID)
=======
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#include <linux/irq.h>
extern void dhdsdio_isr(void * args);
#include <bcmutils.h>
#include <dngl_stats.h>
#include <dhd.h>
<<<<<<< HEAD
#endif /* defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID) */

/**
 * SDIO Host Controller info
 */
typedef struct bcmsdh_hc bcmsdh_hc_t;

struct bcmsdh_hc {
	bcmsdh_hc_t *next;
#ifdef BCMPLATFORM_BUS
	struct device *dev;			/* platform device handle */
#else
	struct pci_dev *dev;		/* pci device handle */
#endif /* BCMPLATFORM_BUS */
	osl_t *osh;
	void *regs;			/* SDIO Host Controller address */
	bcmsdh_info_t *sdh;		/* SDIO Host Controller handle */
	void *ch;
	unsigned int oob_irq;
	unsigned long oob_flags; /* OOB Host specifiction as edge and etc */
	bool oob_irq_registered;
	bool oob_irq_enable_flag;
#if defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID)
	spinlock_t irq_lock;
#endif /* defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID) */
};
static bcmsdh_hc_t *sdhcinfo = NULL;

struct device *pm_dev;

/* driver info, initialized when bcmsdh_register is called */
static bcmsdh_driver_t drvinfo = {NULL, NULL};
=======

#include <dhd_linux.h>

/* driver info, initialized when bcmsdh_register is called */
static bcmsdh_driver_t drvinfo = {NULL, NULL, NULL, NULL};

typedef enum {
	DHD_INTR_INVALID = 0,
	DHD_INTR_INBAND,
	DHD_INTR_HWOOB,
	DHD_INTR_SWOOB
} DHD_HOST_INTR_TYPE;

/* the BCMSDH module comprises the generic part (bcmsdh.c) and OS specific layer (e.g.
 * bcmsdh_linux.c). Put all OS specific variables (e.g. irq number and flags) here rather
 * than in the common structure bcmsdh_info. bcmsdh_info only keeps a handle (os_ctx) to this
 * structure.
 */
typedef struct bcmsdh_os_info {
	DHD_HOST_INTR_TYPE	intr_type;
	int			oob_irq_num;	/* valid when hardware or software oob in use */
	unsigned long		oob_irq_flags;	/* valid when hardware or software oob in use */
	bool			oob_irq_registered;
	bool			oob_irq_enabled;
	bool			oob_irq_wake_enabled;
	spinlock_t		oob_irq_spinlock;
	bcmsdh_cb_fn_t		oob_irq_handler;
	void			*oob_irq_handler_context;
	void			*context;	/* context returned from upper layer */
	void			*sdioh;		/* handle to lower layer (sdioh) */
	void			*dev;		/* handle to the underlying device */
	bool			dev_wake_enabled;
} bcmsdh_os_info_t;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

/* debugging macros */
#define SDLX_MSG(x)

/**
 * Checks to see if vendor and device IDs match a supported SDIO Host Controller.
 */
bool
bcmsdh_chipmatch(uint16 vendor, uint16 device)
{
	/* Add other vendors and devices as required */

#ifdef BCMSDIOH_STD
	/* Check for Arasan host controller */
	if (vendor == VENDOR_SI_IMAGE) {
		return (TRUE);
	}
	/* Check for BRCM 27XX Standard host controller */
	if (device == BCM27XX_SDIOH_ID && vendor == VENDOR_BROADCOM) {
		return (TRUE);
	}
	/* Check for BRCM Standard host controller */
	if (device == SDIOH_FPGA_ID && vendor == VENDOR_BROADCOM) {
		return (TRUE);
	}
	/* Check for TI PCIxx21 Standard host controller */
	if (device == PCIXX21_SDIOH_ID && vendor == VENDOR_TI) {
		return (TRUE);
	}
	if (device == PCIXX21_SDIOH0_ID && vendor == VENDOR_TI) {
		return (TRUE);
	}
	/* Ricoh R5C822 Standard SDIO Host */
	if (device == R5C822_SDIOH_ID && vendor == VENDOR_RICOH) {
		return (TRUE);
	}
	/* JMicron Standard SDIO Host */
	if (device == JMICRON_SDIOH_ID && vendor == VENDOR_JMICRON) {
		return (TRUE);
	}

#endif /* BCMSDIOH_STD */
#ifdef BCMSDIOH_SPI
	/* This is the PciSpiHost. */
	if (device == SPIH_FPGA_ID && vendor == VENDOR_BROADCOM) {
		printf("Found PCI SPI Host Controller\n");
		return (TRUE);
	}

#endif /* BCMSDIOH_SPI */

	return (FALSE);
}

<<<<<<< HEAD
#if defined(BCMPLATFORM_BUS)
#if defined(BCMLXSDMMC) || defined(BCMSPI_ANDROID)
/* forward declarations */
int bcmsdh_probe(struct device *dev);
int bcmsdh_remove(struct device *dev);

EXPORT_SYMBOL(bcmsdh_probe);
EXPORT_SYMBOL(bcmsdh_remove);

#else
/* forward declarations */
static int __devinit bcmsdh_probe(struct device *dev);
static int __devexit bcmsdh_remove(struct device *dev);
#endif /* defined(BCMLXSDMMC) || defined(BCMSPI_ANDROID) */

#if !defined(BCMLXSDMMC) && !defined(BCMSPI_ANDROID)
static
#endif /* !defined(BCMLXSDMMC) && !defined(BCMSPI_ANDROID) */
int bcmsdh_probe(struct device *dev)
{
	osl_t *osh = NULL;
	bcmsdh_hc_t *sdhc = NULL, *sdhc_org = sdhcinfo;
	ulong regs = 0;
	bcmsdh_info_t *sdh = NULL;
#if !defined(BCMLXSDMMC) && defined(BCMPLATFORM_BUS) && !defined(BCMSPI_ANDROID)
	struct platform_device *pdev;
	struct resource *r;
#endif /* !defined(BCMLXSDMMC) && defined(BCMPLATFORM_BUS) && !defined(BCMSPI_ANDROID) */
	int irq = 0;
	uint32 vendevid;
	unsigned long irq_flags = 0;

#if !defined(BCMLXSDMMC) && defined(BCMPLATFORM_BUS) && !defined(BCMSPI_ANDROID)
	pdev = to_platform_device(dev);
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	irq = platform_get_irq(pdev, 0);
	if (!r || irq < 0)
		return -ENXIO;
#endif /* !defined(BCMLXSDMMC) && defined(BCMPLATFORM_BUS) && !defined(BCMSPI_ANDROID) */

#if defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID)
#ifdef HW_OOB
	irq_flags =
		IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHLEVEL | IORESOURCE_IRQ_SHAREABLE;
#else
	 irq_flags = IRQF_TRIGGER_FALLING;
#endif /* HW_OOB */

	/* Get customer specific OOB IRQ parametres: IRQ number as IRQ type */
	irq = dhd_customer_oob_irq_map(&irq_flags);
	if  (irq < 0) {
		SDLX_MSG(("%s: Host irq is not defined\n", __FUNCTION__));
		goto err;
	}
#endif /* defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID) */
	/* allocate SDIO Host Controller state info */
	if (!(osh = osl_attach(dev, PCI_BUS, FALSE))) {
		SDLX_MSG(("%s: osl_attach failed\n", __FUNCTION__));
		goto err;
	}
	if (!(sdhc = MALLOC(osh, sizeof(bcmsdh_hc_t)))) {
		SDLX_MSG(("%s: out of memory, allocated %d bytes\n",
			__FUNCTION__,
			MALLOCED(osh)));
		goto err;
	}
	bzero(sdhc, sizeof(bcmsdh_hc_t));
	sdhc->osh = osh;

	sdhc->dev = (void *)dev;

#if defined(BCMLXSDMMC) || defined(BCMSPI_ANDROID)
	if (!(sdh = bcmsdh_attach(osh, (void *)0,
	                          (void **)&regs, irq))) {
		SDLX_MSG(("%s: bcmsdh_attach failed\n", __FUNCTION__));
		goto err;
	}
#else
	if (!(sdh = bcmsdh_attach(osh, (void *)r->start,
	                          (void **)&regs, irq))) {
		SDLX_MSG(("%s: bcmsdh_attach failed\n", __FUNCTION__));
		goto err;
	}
#endif /* defined(BCMLXSDMMC) || defined(BCMSPI_ANDROID) */
	sdhc->sdh = sdh;
	sdhc->oob_irq = irq;
	sdhc->oob_flags = irq_flags;
	sdhc->oob_irq_registered = FALSE;	/* to make sure.. */
	sdhc->oob_irq_enable_flag = FALSE;
#if defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID)
	spin_lock_init(&sdhc->irq_lock);
#endif /* defined(BCMLXSDMMC) || defined(BCMSPI_ANDROID) */

	/* chain SDIO Host Controller info together */
	sdhc->next = sdhcinfo;
	sdhcinfo = sdhc;

#if !defined(CONFIG_HAS_WAKELOCK) && (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36))
	if (!device_init_wakeup(dev, 1))
		pm_dev = dev;
#endif /* !CONFIG_HAS_WAKELOCK */

	/* Read the vendor/device ID from the CIS */
	vendevid = bcmsdh_query_device(sdh);
	/* try to attach to the target device */
	if (!(sdhc->ch = drvinfo.attach((vendevid >> 16),
	                                 (vendevid & 0xFFFF), 0, 0, 0, 0,
	                                (void *)regs, NULL, sdh))) {
=======
void* bcmsdh_probe(osl_t *osh, void *dev, void *sdioh, void *adapter_info, uint bus_type,
	uint bus_num, uint slot_num)
{
	ulong regs;
	bcmsdh_info_t *bcmsdh;
	uint32 vendevid;
	bcmsdh_os_info_t *bcmsdh_osinfo = NULL;

	bcmsdh = bcmsdh_attach(osh, sdioh, &regs);
	if (bcmsdh == NULL) {
		SDLX_MSG(("%s: bcmsdh_attach failed\n", __FUNCTION__));
		goto err;
	}
	bcmsdh_osinfo = MALLOC(osh, sizeof(bcmsdh_os_info_t));
	if (bcmsdh_osinfo == NULL) {
		SDLX_MSG(("%s: failed to allocate bcmsdh_os_info_t\n", __FUNCTION__));
		goto err;
	}
	bzero((char *)bcmsdh_osinfo, sizeof(bcmsdh_os_info_t));
	bcmsdh->os_cxt = bcmsdh_osinfo;
	bcmsdh_osinfo->sdioh = sdioh;
	bcmsdh_osinfo->dev = dev;
	osl_set_bus_handle(osh, bcmsdh);

#if !defined(CONFIG_HAS_WAKELOCK) && (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36))
	if (dev && device_init_wakeup(dev, true) == 0)
		bcmsdh_osinfo->dev_wake_enabled = TRUE;
#endif /* !defined(CONFIG_HAS_WAKELOCK) && (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36)) */

#if defined(OOB_INTR_ONLY)
	spin_lock_init(&bcmsdh_osinfo->oob_irq_spinlock);
	/* Get customer specific OOB IRQ parametres: IRQ number as IRQ type */
	bcmsdh_osinfo->oob_irq_num = wifi_platform_get_irq_number(adapter_info,
		&bcmsdh_osinfo->oob_irq_flags);
	if  (bcmsdh_osinfo->oob_irq_num < 0) {
		SDLX_MSG(("%s: Host OOB irq is not defined\n", __FUNCTION__));
		goto err;
	}
#endif /* defined(BCMLXSDMMC) */

	/* Read the vendor/device ID from the CIS */
	vendevid = bcmsdh_query_device(bcmsdh);
	/* try to attach to the target device */
	bcmsdh_osinfo->context = drvinfo.probe((vendevid >> 16), (vendevid & 0xFFFF), bus_num,
		slot_num, 0, bus_type, (void *)regs, osh, bcmsdh);
	if (bcmsdh_osinfo->context == NULL) {
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		SDLX_MSG(("%s: device attach failed\n", __FUNCTION__));
		goto err;
	}

<<<<<<< HEAD
	return 0;

	/* error handling */
err:
	if (sdhc) {
		if (sdhc->sdh)
			bcmsdh_detach(sdhc->osh, sdhc->sdh);
		MFREE(osh, sdhc, sizeof(bcmsdh_hc_t));
		sdhcinfo = sdhc_org;
	}
	if (osh)
		osl_detach(osh);
	return -ENODEV;
}

#if !defined(BCMLXSDMMC) && !defined(BCMSPI_ANDROID)
static
#endif /* !defined(BCMLXSDMMC) && !defined(BCMSPI_ANDROID) */
int bcmsdh_remove(struct device *dev)
{
	bcmsdh_hc_t *sdhc, *prev;
	osl_t *osh;
	int sdhcinfo_null = false;

	/* find the SDIO Host Controller state for this pdev and take it out from the list */
	for (sdhc = sdhcinfo, prev = NULL; sdhc; sdhc = sdhc->next) {
		if (sdhc->dev == (void *)dev) {
			if (prev)
				prev->next = sdhc->next;
			else {
				if (sdhc->next != NULL) {
					SDLX_MSG(("%s: more SDHC exist, should be care about it\n",
						__FUNCTION__));
				}
				sdhcinfo_null = true;
			}
			break;
		}
		prev = sdhc;
	}
	if (!sdhc) {
		SDLX_MSG(("%s: failed\n", __FUNCTION__));
		return 0;
	}

	/* detach ch & sdhc if dev is valid */
	drvinfo.detach(sdhc->ch);
	bcmsdh_detach(sdhc->osh, sdhc->sdh);

#if !defined(CONFIG_HAS_WAKELOCK) && (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36))
	if (pm_dev) {
		device_init_wakeup(pm_dev, 0);
		pm_dev = NULL;
	}
#endif /* !CONFIG_HAS_WAKELOCK */

	if (sdhcinfo_null == true)
		sdhcinfo = NULL;

	/* release SDIO Host Controller info */
	osh = sdhc->osh;
	MFREE(osh, sdhc, sizeof(bcmsdh_hc_t));
	osl_detach(osh);

#if !defined(BCMLXSDMMC) || defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID)
	dev_set_drvdata(dev, NULL);
#endif /* !defined(BCMLXSDMMC) || defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID) */
=======
	return bcmsdh;

	/* error handling */
err:
	if (bcmsdh != NULL)
		bcmsdh_detach(osh, bcmsdh);
	if (bcmsdh_osinfo != NULL)
		MFREE(osh, bcmsdh_osinfo, sizeof(bcmsdh_os_info_t));
	return NULL;
}

int bcmsdh_remove(bcmsdh_info_t *bcmsdh)
{
	bcmsdh_os_info_t *bcmsdh_osinfo = bcmsdh->os_cxt;

#if !defined(CONFIG_HAS_WAKELOCK) && (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36))
	if (bcmsdh_osinfo->dev)
		device_init_wakeup(bcmsdh_osinfo->dev, false);
	bcmsdh_osinfo->dev_wake_enabled = FALSE;
#endif /* !defined(CONFIG_HAS_WAKELOCK) && (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36)) */

	drvinfo.remove(bcmsdh_osinfo->context);
	MFREE(bcmsdh->osh, bcmsdh->os_cxt, sizeof(bcmsdh_os_info_t));
	bcmsdh_detach(bcmsdh->osh, bcmsdh);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

	return 0;
}

<<<<<<< HEAD
#else /* BCMPLATFORM_BUS */

#if !defined(BCMLXSDMMC)
/* forward declarations for PCI probe and remove functions. */
static int __devinit bcmsdh_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent);
static void __devexit bcmsdh_pci_remove(struct pci_dev *pdev);

/**
 * pci id table
 */
static struct pci_device_id bcmsdh_pci_devid[] __devinitdata = {
	{ vendor: PCI_ANY_ID,
	device: PCI_ANY_ID,
	subvendor: PCI_ANY_ID,
	subdevice: PCI_ANY_ID,
	class: 0,
	class_mask: 0,
	driver_data: 0,
	},
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, bcmsdh_pci_devid);

/**
 * SDIO Host Controller pci driver info
 */
static struct pci_driver bcmsdh_pci_driver = {
	node:		{},
	name:		"bcmsdh",
	id_table:	bcmsdh_pci_devid,
	probe:		bcmsdh_pci_probe,
	remove:		bcmsdh_pci_remove,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
	save_state:	NULL,
#endif
	suspend:	NULL,
	resume:		NULL,
	};


extern uint sd_pci_slot;	/* Force detection to a particular PCI */
							/* slot only . Allows for having multiple */
							/* WL devices at once in a PC */
							/* Only one instance of dhd will be */
							/* usable at a time */
							/* Upper word is bus number, */
							/* lower word is slot number */
							/* Default value of 0xffffffff turns this */
							/* off */
module_param(sd_pci_slot, uint, 0);


/**
 * Detect supported SDIO Host Controller and attach if found.
 *
 * Determine if the device described by pdev is a supported SDIO Host
 * Controller.  If so, attach to it and attach to the target device.
 */
static int __devinit
bcmsdh_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	osl_t *osh = NULL;
	bcmsdh_hc_t *sdhc = NULL;
	ulong regs;
	bcmsdh_info_t *sdh = NULL;
	int rc;

	if (sd_pci_slot != 0xFFFFffff) {
		if (pdev->bus->number != (sd_pci_slot>>16) ||
			PCI_SLOT(pdev->devfn) != (sd_pci_slot&0xffff)) {
			SDLX_MSG(("%s: %s: bus %X, slot %X, vend %X, dev %X\n",
				__FUNCTION__,
				bcmsdh_chipmatch(pdev->vendor, pdev->device)
				?"Found compatible SDIOHC"
				:"Probing unknown device",
				pdev->bus->number, PCI_SLOT(pdev->devfn), pdev->vendor,
				pdev->device));
			return -ENODEV;
		}
		SDLX_MSG(("%s: %s: bus %X, slot %X, vendor %X, device %X (good PCI location)\n",
			__FUNCTION__,
			bcmsdh_chipmatch(pdev->vendor, pdev->device)
			?"Using compatible SDIOHC"
			:"WARNING, forced use of unkown device",
			pdev->bus->number, PCI_SLOT(pdev->devfn), pdev->vendor, pdev->device));
	}

	if ((pdev->vendor == VENDOR_TI) && ((pdev->device == PCIXX21_FLASHMEDIA_ID) ||
	    (pdev->device == PCIXX21_FLASHMEDIA0_ID))) {
		uint32 config_reg;

		SDLX_MSG(("%s: Disabling TI FlashMedia Controller.\n", __FUNCTION__));
		if (!(osh = osl_attach(pdev, PCI_BUS, FALSE))) {
			SDLX_MSG(("%s: osl_attach failed\n", __FUNCTION__));
			goto err;
		}

		config_reg = OSL_PCI_READ_CONFIG(osh, 0x4c, 4);

		/*
		 * Set MMC_SD_DIS bit in FlashMedia Controller.
		 * Disbling the SD/MMC Controller in the FlashMedia Controller
		 * allows the Standard SD Host Controller to take over control
		 * of the SD Slot.
		 */
		config_reg |= 0x02;
		OSL_PCI_WRITE_CONFIG(osh, 0x4c, 4, config_reg);
		osl_detach(osh);
	}
	/* match this pci device with what we support */
	/* we can't solely rely on this to believe it is our SDIO Host Controller! */
	if (!bcmsdh_chipmatch(pdev->vendor, pdev->device)) {
		if (pdev->vendor == VENDOR_BROADCOM) {
			SDLX_MSG(("%s: Unknown Broadcom device (vendor: %#x, device: %#x).\n",
				__FUNCTION__, pdev->vendor, pdev->device));
		}
		return -ENODEV;
	}

	/* this is a pci device we might support */
	SDLX_MSG(("%s: Found possible SDIO Host Controller: bus %d slot %d func %d irq %d\n",
		__FUNCTION__,
		pdev->bus->number, PCI_SLOT(pdev->devfn),
		PCI_FUNC(pdev->devfn), pdev->irq));

	/* use bcmsdh_query_device() to get the vendor ID of the target device so
	 * it will eventually appear in the Broadcom string on the console
	 */

	/* allocate SDIO Host Controller state info */
	if (!(osh = osl_attach(pdev, PCI_BUS, FALSE))) {
		SDLX_MSG(("%s: osl_attach failed\n", __FUNCTION__));
		goto err;
	}
	if (!(sdhc = MALLOC(osh, sizeof(bcmsdh_hc_t)))) {
		SDLX_MSG(("%s: out of memory, allocated %d bytes\n",
			__FUNCTION__,
			MALLOCED(osh)));
		goto err;
	}
	bzero(sdhc, sizeof(bcmsdh_hc_t));
	sdhc->osh = osh;

	sdhc->dev = pdev;

	/* map to address where host can access */
	pci_set_master(pdev);
	rc = pci_enable_device(pdev);
	if (rc) {
		SDLX_MSG(("%s: Cannot enable PCI device\n", __FUNCTION__));
		goto err;
	}
	if (!(sdh = bcmsdh_attach(osh, (void *)(uintptr)pci_resource_start(pdev, 0),
	                          (void **)&regs, pdev->irq))) {
		SDLX_MSG(("%s: bcmsdh_attach failed\n", __FUNCTION__));
		goto err;
	}

	sdhc->sdh = sdh;

	/* try to attach to the target device */
	if (!(sdhc->ch = drvinfo.attach(VENDOR_BROADCOM, /* pdev->vendor, */
	                                bcmsdh_query_device(sdh) & 0xFFFF, 0, 0, 0, 0,
	                                (void *)regs, NULL, sdh))) {
		SDLX_MSG(("%s: device attach failed\n", __FUNCTION__));
		goto err;
	}

	/* chain SDIO Host Controller info together */
	sdhc->next = sdhcinfo;
	sdhcinfo = sdhc;

	return 0;

	/* error handling */
err:
	if (sdhc) {
		if (sdhc->sdh)
			bcmsdh_detach(sdhc->osh, sdhc->sdh);
		MFREE(osh, sdhc, sizeof(bcmsdh_hc_t));
	}
	if (osh)
		osl_detach(osh);
	return -ENODEV;
}


/**
 * Detach from target devices and SDIO Host Controller
 */
static void __devexit
bcmsdh_pci_remove(struct pci_dev *pdev)
{
	bcmsdh_hc_t *sdhc, *prev;
	osl_t *osh;

	/* find the SDIO Host Controller state for this pdev and take it out from the list */
	for (sdhc = sdhcinfo, prev = NULL; sdhc; sdhc = sdhc->next) {
		if (sdhc->dev == pdev) {
			if (prev)
				prev->next = sdhc->next;
			else
				sdhcinfo = NULL;
			break;
		}
		prev = sdhc;
	}
	if (!sdhc)
		return;

	drvinfo.detach(sdhc->ch);

	bcmsdh_detach(sdhc->osh, sdhc->sdh);

	/* release SDIO Host Controller info */
	osh = sdhc->osh;
	MFREE(osh, sdhc, sizeof(bcmsdh_hc_t));
	osl_detach(osh);
}
#endif /* BCMLXSDMMC */
#endif /* BCMPLATFORM_BUS */

#ifdef BCMSPI_ANDROID
extern int spi_function_init(void);
#else
extern int sdio_function_init(void);
#endif /* BCMSPI_ANDROID */

=======
int bcmsdh_suspend(bcmsdh_info_t *bcmsdh)
{
	bcmsdh_os_info_t *bcmsdh_osinfo = bcmsdh->os_cxt;

	if (drvinfo.suspend && drvinfo.suspend(bcmsdh_osinfo->context))
		return -EBUSY;
	return 0;
}

int bcmsdh_resume(bcmsdh_info_t *bcmsdh)
{
	bcmsdh_os_info_t *bcmsdh_osinfo = bcmsdh->os_cxt;

	if (drvinfo.resume)
		return drvinfo.resume(bcmsdh_osinfo->context);
	return 0;
}

extern int bcmsdh_register_client_driver(void);
extern void bcmsdh_unregister_client_driver(void);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
extern int sdio_func_reg_notify(void* semaphore);
extern void sdio_func_unreg_notify(void);

#if defined(BCMLXSDMMC)
int bcmsdh_reg_sdio_notify(void* semaphore)
{
	return sdio_func_reg_notify(semaphore);
}

void bcmsdh_unreg_sdio_notify(void)
{
	sdio_func_unreg_notify();
}
#endif /* defined(BCMLXSDMMC) */

int
bcmsdh_register(bcmsdh_driver_t *driver)
{
	int error = 0;

	drvinfo = *driver;
<<<<<<< HEAD

#if defined(BCMPLATFORM_BUS)
#ifdef BCMSPI_ANDROID
	SDLX_MSG(("Linux Kernel SPI Driver\n"));
	error = spi_function_init();
#else /* BCMSPI_ANDROID */
	SDLX_MSG(("Linux Kernel SDIO/MMC Driver\n"));
	error = sdio_function_init();
#endif /* BCMSPI_ANDROID */
	return error;
#endif /* defined(BCMPLATFORM_BUS) */

#if !defined(BCMPLATFORM_BUS) && !defined(BCMLXSDMMC)
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
	if (!(error = pci_module_init(&bcmsdh_pci_driver)))
		return 0;
#else
	if (!(error = pci_register_driver(&bcmsdh_pci_driver)))
		return 0;
#endif

	SDLX_MSG(("%s: pci_module_init failed 0x%x\n", __FUNCTION__, error));
#endif /* BCMPLATFORM_BUS */

	return error;
}

#ifdef BCMSPI_ANDROID
extern void spi_function_cleanup(void);
#else
extern void sdio_function_cleanup(void);
#endif /* BCMSPI_ANDROID */

void
bcmsdh_unregister(void)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
	if (bcmsdh_pci_driver.node.next)
#endif

#ifdef BCMSPI_ANDROID
	spi_function_cleanup();
#endif /* BCMSPI_ANDROID */
#if defined(BCMLXSDMMC)
	sdio_function_cleanup();
#endif /* BCMLXSDMMC */

#if !defined(BCMPLATFORM_BUS) && !defined(BCMLXSDMMC)
	pci_unregister_driver(&bcmsdh_pci_driver);
#endif /* BCMPLATFORM_BUS */
}

#if defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID)
void bcmsdh_oob_intr_set(bool enable)
{
	static bool curstate = 1;
	unsigned long flags;

	spin_lock_irqsave(&sdhcinfo->irq_lock, flags);
	if (curstate != enable) {
		if (enable)
			enable_irq(sdhcinfo->oob_irq);
		else
			disable_irq_nosync(sdhcinfo->oob_irq);
		curstate = enable;
	}
	spin_unlock_irqrestore(&sdhcinfo->irq_lock, flags);
=======
	SDLX_MSG(("%s: register client driver\n", __FUNCTION__));
	error = bcmsdh_register_client_driver();
	if (error)
		SDLX_MSG(("%s: failed %d\n", __FUNCTION__, error));

	return error;
}

void
bcmsdh_unregister(void)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
		if (bcmsdh_pci_driver.node.next == NULL)
			return;
#endif

	bcmsdh_unregister_client_driver();
}

void bcmsdh_dev_pm_stay_awake(bcmsdh_info_t *bcmsdh)
{
#if !defined(CONFIG_HAS_WAKELOCK) && (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36))
	bcmsdh_os_info_t *bcmsdh_osinfo = bcmsdh->os_cxt;
	pm_stay_awake(bcmsdh_osinfo->dev);
#endif /* !defined(CONFIG_HAS_WAKELOCK) && (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36)) */
}

void bcmsdh_dev_relax(bcmsdh_info_t *bcmsdh)
{
#if !defined(CONFIG_HAS_WAKELOCK) && (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36))
	bcmsdh_os_info_t *bcmsdh_osinfo = bcmsdh->os_cxt;
	pm_relax(bcmsdh_osinfo->dev);
#endif /* !defined(CONFIG_HAS_WAKELOCK) && (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36)) */
}

bool bcmsdh_dev_pm_enabled(bcmsdh_info_t *bcmsdh)
{
	bcmsdh_os_info_t *bcmsdh_osinfo = bcmsdh->os_cxt;

	return bcmsdh_osinfo->dev_wake_enabled;
}

#if defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID)
void bcmsdh_oob_intr_set(bcmsdh_info_t *bcmsdh, bool enable)
{
	unsigned long flags;
	bcmsdh_os_info_t *bcmsdh_osinfo;

	if (!bcmsdh)
		return;

	bcmsdh_osinfo = bcmsdh->os_cxt;
	spin_lock_irqsave(&bcmsdh_osinfo->oob_irq_spinlock, flags);
	if (bcmsdh_osinfo->oob_irq_enabled != enable) {
		if (enable)
			enable_irq(bcmsdh_osinfo->oob_irq_num);
		else
			disable_irq_nosync(bcmsdh_osinfo->oob_irq_num);
		bcmsdh_osinfo->oob_irq_enabled = enable;
	}
	spin_unlock_irqrestore(&bcmsdh_osinfo->oob_irq_spinlock, flags);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
}

static irqreturn_t wlan_oob_irq(int irq, void *dev_id)
{
<<<<<<< HEAD
	dhd_pub_t *dhdp;

	dhdp = (dhd_pub_t *)dev_get_drvdata(sdhcinfo->dev);

#ifndef BCMSPI_ANDROID
	bcmsdh_oob_intr_set(0);
#endif /* !BCMSPI_ANDROID */

	if (dhdp == NULL) {
		SDLX_MSG(("Out of band GPIO interrupt fired way too early\n"));
		return IRQ_HANDLED;
	}

	dhdsdio_isr((void *)dhdp->bus);
=======
	bcmsdh_info_t *bcmsdh = (bcmsdh_info_t *)dev_id;
	bcmsdh_os_info_t *bcmsdh_osinfo = bcmsdh->os_cxt;

#ifndef BCMSPI_ANDROID
	bcmsdh_oob_intr_set(bcmsdh, FALSE);
#endif /* !BCMSPI_ANDROID */
	bcmsdh_osinfo->oob_irq_handler(bcmsdh_osinfo->oob_irq_handler_context);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

	return IRQ_HANDLED;
}

<<<<<<< HEAD
int bcmsdh_register_oob_intr(void * dhdp)
{
	int error = 0;

	SDLX_MSG(("%s Enter \n", __FUNCTION__));

	/* IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHLEVEL | IORESOURCE_IRQ_SHAREABLE; */

	dev_set_drvdata(sdhcinfo->dev, dhdp);

	if (!sdhcinfo->oob_irq_registered) {
		SDLX_MSG(("%s IRQ=%d Type=%X \n", __FUNCTION__,
			(int)sdhcinfo->oob_irq, (int)sdhcinfo->oob_flags));
		/* Refer to customer Host IRQ docs about proper irqflags definition */
		error = request_irq(sdhcinfo->oob_irq, wlan_oob_irq, sdhcinfo->oob_flags,
			"bcmsdh_sdmmc", NULL);
		if (error)
			return -ENODEV;

#if defined(CONFIG_ARCH_RHEA) || defined(CONFIG_ARCH_CAPRI)
		if (device_may_wakeup(sdhcinfo->dev)) {
#endif
			error = enable_irq_wake(sdhcinfo->oob_irq);
#if defined(CONFIG_ARCH_RHEA) || defined(CONFIG_ARCH_CAPRI)
		}
#endif
		if (error)
			SDLX_MSG(("%s enable_irq_wake error=%d \n", __FUNCTION__, error));
		sdhcinfo->oob_irq_registered = TRUE;
		sdhcinfo->oob_irq_enable_flag = TRUE;
	}

	return 0;
}

void bcmsdh_set_irq(int flag)
{
	if (sdhcinfo->oob_irq_registered && sdhcinfo->oob_irq_enable_flag != flag) {
		SDLX_MSG(("%s Flag = %d", __FUNCTION__, flag));
		sdhcinfo->oob_irq_enable_flag = flag;
		if (flag) {
			enable_irq(sdhcinfo->oob_irq);
#if defined(CONFIG_ARCH_RHEA) || defined(CONFIG_ARCH_CAPRI)
			if (device_may_wakeup(sdhcinfo->dev))
#endif
				enable_irq_wake(sdhcinfo->oob_irq);
		} else {
#if !(defined(BCMSPI_ANDROID) && defined(CUSTOMER_HW4) && defined(CONFIG_NKERNEL))
#if defined(CONFIG_ARCH_RHEA) || defined(CONFIG_ARCH_CAPRI)
			if (device_may_wakeup(sdhcinfo->dev))
#endif
				disable_irq_wake(sdhcinfo->oob_irq);
#endif /* !defined(BCMSPI_ANDROID) */
			disable_irq(sdhcinfo->oob_irq);
		}
	}
}

void bcmsdh_unregister_oob_intr(void)
{
	SDLX_MSG(("%s: Enter\n", __FUNCTION__));

	if (sdhcinfo->oob_irq_registered == TRUE) {
		bcmsdh_set_irq(FALSE);
		free_irq(sdhcinfo->oob_irq, NULL);
		sdhcinfo->oob_irq_registered = FALSE;
	}
}

bool bcmsdh_is_oob_intr_registered(void)
{
	if (sdhcinfo)
		return sdhcinfo->oob_irq_registered;
	else
		return FALSE;
}
#endif /* defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID) */

#if defined(BCMLXSDMMC)
void *bcmsdh_get_drvdata(void)
{
	if (!sdhcinfo)
		return NULL;
	return dev_get_drvdata(sdhcinfo->dev);
}
#endif

=======
int bcmsdh_oob_intr_register(bcmsdh_info_t *bcmsdh, bcmsdh_cb_fn_t oob_irq_handler,
	void* oob_irq_handler_context)
{
	int err = 0;
	bcmsdh_os_info_t *bcmsdh_osinfo = bcmsdh->os_cxt;

	SDLX_MSG(("%s: Enter\n", __FUNCTION__));
	if (bcmsdh_osinfo->oob_irq_registered) {
		SDLX_MSG(("%s: irq is already registered\n", __FUNCTION__));
		return -EBUSY;
	}
	SDLX_MSG(("%s OOB irq=%d flags=%X \n", __FUNCTION__,
		(int)bcmsdh_osinfo->oob_irq_num, (int)bcmsdh_osinfo->oob_irq_flags));
	bcmsdh_osinfo->oob_irq_handler = oob_irq_handler;
	bcmsdh_osinfo->oob_irq_handler_context = oob_irq_handler_context;
	err = request_irq(bcmsdh_osinfo->oob_irq_num, wlan_oob_irq,
		bcmsdh_osinfo->oob_irq_flags, "bcmsdh_sdmmc", bcmsdh);
	if (err) {
		SDLX_MSG(("%s: request_irq failed with %d\n", __FUNCTION__, err));
		return err;
	}

#if defined(CONFIG_ARCH_RHEA) || defined(CONFIG_ARCH_CAPRI)
	if (device_may_wakeup(bcmsdh_osinfo->dev)) {
#endif /* CONFIG_ARCH_RHEA || CONFIG_ARCH_CAPRI */
		err = enable_irq_wake(bcmsdh_osinfo->oob_irq_num);
		if (!err)
			bcmsdh_osinfo->oob_irq_wake_enabled = TRUE;
#if defined(CONFIG_ARCH_RHEA) || defined(CONFIG_ARCH_CAPRI)
	}
#endif /* CONFIG_ARCH_RHEA || CONFIG_ARCH_CAPRI */
	bcmsdh_osinfo->oob_irq_enabled = TRUE;
	bcmsdh_osinfo->oob_irq_registered = TRUE;
	return err;
}

void bcmsdh_oob_intr_unregister(bcmsdh_info_t *bcmsdh)
{
	int err = 0;
	bcmsdh_os_info_t *bcmsdh_osinfo = bcmsdh->os_cxt;

	SDLX_MSG(("%s: Enter\n", __FUNCTION__));
	if (!bcmsdh_osinfo->oob_irq_registered) {
		SDLX_MSG(("%s: irq is not registered\n", __FUNCTION__));
		return;
	}
	if (bcmsdh_osinfo->oob_irq_wake_enabled) {
#if defined(CONFIG_ARCH_RHEA) || defined(CONFIG_ARCH_CAPRI)
		if (device_may_wakeup(bcmsdh_osinfo->dev)) {
#endif /* CONFIG_ARCH_RHEA || CONFIG_ARCH_CAPRI */
			err = disable_irq_wake(bcmsdh_osinfo->oob_irq_num);
			if (!err)
				bcmsdh_osinfo->oob_irq_wake_enabled = FALSE;
#if defined(CONFIG_ARCH_RHEA) || defined(CONFIG_ARCH_CAPRI)
		}
#endif /* CONFIG_ARCH_RHEA || CONFIG_ARCH_CAPRI */
	}
	if (bcmsdh_osinfo->oob_irq_enabled) {
		disable_irq(bcmsdh_osinfo->oob_irq_num);
		bcmsdh_osinfo->oob_irq_enabled = FALSE;
	}
	free_irq(bcmsdh_osinfo->oob_irq_num, bcmsdh);
	bcmsdh_osinfo->oob_irq_registered = FALSE;
}
#endif /* defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID) */

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
/* Module parameters specific to each host-controller driver */

extern uint sd_msglevel;	/* Debug message level */
module_param(sd_msglevel, uint, 0);

extern uint sd_power;	/* 0 = SD Power OFF, 1 = SD Power ON. */
module_param(sd_power, uint, 0);

extern uint sd_clock;	/* SD Clock Control, 0 = SD Clock OFF, 1 = SD Clock ON */
module_param(sd_clock, uint, 0);

extern uint sd_divisor;	/* Divisor (-1 means external clock) */
module_param(sd_divisor, uint, 0);

extern uint sd_sdmode;	/* Default is SD4, 0=SPI, 1=SD1, 2=SD4 */
module_param(sd_sdmode, uint, 0);

extern uint sd_hiok;	/* Ok to use hi-speed mode */
module_param(sd_hiok, uint, 0);

extern uint sd_f2_blocksize;
module_param(sd_f2_blocksize, int, 0);

#ifdef BCMSDIOH_STD
extern int sd_uhsimode;
module_param(sd_uhsimode, int, 0);
extern uint sd_tuning_period;
module_param(sd_tuning_period, uint, 0);
extern int sd_delay_value;
module_param(sd_delay_value, uint, 0);
<<<<<<< HEAD
#endif

#ifdef BCMSDIOH_TXGLOM
extern uint sd_txglom;
module_param(sd_txglom, uint, 0);
=======

/* SDIO Drive Strength for UHSI mode specific to SDIO3.0 */
extern char dhd_sdiod_uhsi_ds_override[2];
module_param_string(dhd_sdiod_uhsi_ds_override, dhd_sdiod_uhsi_ds_override, 2, 0);

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#endif

#ifdef BCMSDH_MODULE
EXPORT_SYMBOL(bcmsdh_attach);
EXPORT_SYMBOL(bcmsdh_detach);
EXPORT_SYMBOL(bcmsdh_intr_query);
EXPORT_SYMBOL(bcmsdh_intr_enable);
EXPORT_SYMBOL(bcmsdh_intr_disable);
EXPORT_SYMBOL(bcmsdh_intr_reg);
EXPORT_SYMBOL(bcmsdh_intr_dereg);

#if defined(DHD_DEBUG)
EXPORT_SYMBOL(bcmsdh_intr_pending);
#endif

EXPORT_SYMBOL(bcmsdh_devremove_reg);
EXPORT_SYMBOL(bcmsdh_cfg_read);
EXPORT_SYMBOL(bcmsdh_cfg_write);
EXPORT_SYMBOL(bcmsdh_cis_read);
EXPORT_SYMBOL(bcmsdh_reg_read);
EXPORT_SYMBOL(bcmsdh_reg_write);
EXPORT_SYMBOL(bcmsdh_regfail);
EXPORT_SYMBOL(bcmsdh_send_buf);
EXPORT_SYMBOL(bcmsdh_recv_buf);

EXPORT_SYMBOL(bcmsdh_rwdata);
EXPORT_SYMBOL(bcmsdh_abort);
EXPORT_SYMBOL(bcmsdh_query_device);
EXPORT_SYMBOL(bcmsdh_query_iofnum);
EXPORT_SYMBOL(bcmsdh_iovar_op);
EXPORT_SYMBOL(bcmsdh_register);
EXPORT_SYMBOL(bcmsdh_unregister);
EXPORT_SYMBOL(bcmsdh_chipmatch);
EXPORT_SYMBOL(bcmsdh_reset);
EXPORT_SYMBOL(bcmsdh_waitlockfree);

EXPORT_SYMBOL(bcmsdh_get_dstatus);
EXPORT_SYMBOL(bcmsdh_cfg_read_word);
EXPORT_SYMBOL(bcmsdh_cfg_write_word);
EXPORT_SYMBOL(bcmsdh_cur_sbwad);
EXPORT_SYMBOL(bcmsdh_chipinfo);

#endif /* BCMSDH_MODULE */
