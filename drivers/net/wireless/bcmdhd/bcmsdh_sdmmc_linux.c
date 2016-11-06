/*
 * BCMSDH Function Driver for the native SDIO/MMC driver in the Linux Kernel
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
 * $Id: bcmsdh_sdmmc_linux.c 425711 2013-09-25 06:40:41Z $
=======
 * $Id: bcmsdh_sdmmc_linux.c 531050 2015-02-02 07:21:19Z $
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 */

#include <typedefs.h>
#include <bcmutils.h>
#include <sdio.h>	/* SDIO Device and Protocol Specs */
#include <bcmsdbus.h>	/* bcmsdh to/from specific controller APIs */
#include <sdiovar.h>	/* to get msglevel bit values */

#include <linux/sched.h>	/* request_irq() */

#include <linux/mmc/core.h>
#include <linux/mmc/card.h>
<<<<<<< HEAD
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
=======
#include <linux/mmc/host.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
#include <dhd_linux.h>
#include <bcmsdh_sdmmc.h>
#include <dhd_dbg.h>
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

#if !defined(SDIO_VENDOR_ID_BROADCOM)
#define SDIO_VENDOR_ID_BROADCOM		0x02d0
#endif /* !defined(SDIO_VENDOR_ID_BROADCOM) */

#define SDIO_DEVICE_ID_BROADCOM_DEFAULT	0x0000

#if !defined(SDIO_DEVICE_ID_BROADCOM_4325_SDGWB)
#define SDIO_DEVICE_ID_BROADCOM_4325_SDGWB	0x0492	/* BCM94325SDGWB */
#endif /* !defined(SDIO_DEVICE_ID_BROADCOM_4325_SDGWB) */
#if !defined(SDIO_DEVICE_ID_BROADCOM_4325)
#define SDIO_DEVICE_ID_BROADCOM_4325	0x0493
#endif /* !defined(SDIO_DEVICE_ID_BROADCOM_4325) */
#if !defined(SDIO_DEVICE_ID_BROADCOM_4329)
#define SDIO_DEVICE_ID_BROADCOM_4329	0x4329
#endif /* !defined(SDIO_DEVICE_ID_BROADCOM_4329) */
#if !defined(SDIO_DEVICE_ID_BROADCOM_4319)
#define SDIO_DEVICE_ID_BROADCOM_4319	0x4319
#endif /* !defined(SDIO_DEVICE_ID_BROADCOM_4319) */
#if !defined(SDIO_DEVICE_ID_BROADCOM_4330)
#define SDIO_DEVICE_ID_BROADCOM_4330	0x4330
#endif /* !defined(SDIO_DEVICE_ID_BROADCOM_4330) */
#if !defined(SDIO_DEVICE_ID_BROADCOM_4334)
#define SDIO_DEVICE_ID_BROADCOM_4334    0x4334
#endif /* !defined(SDIO_DEVICE_ID_BROADCOM_4334) */
#if !defined(SDIO_DEVICE_ID_BROADCOM_4324)
#define SDIO_DEVICE_ID_BROADCOM_4324    0x4324
#endif /* !defined(SDIO_DEVICE_ID_BROADCOM_4324) */
#if !defined(SDIO_DEVICE_ID_BROADCOM_43239)
#define SDIO_DEVICE_ID_BROADCOM_43239    43239
#endif /* !defined(SDIO_DEVICE_ID_BROADCOM_43239) */

<<<<<<< HEAD

#include <bcmsdh_sdmmc.h>

#include <dhd_dbg.h>

#ifdef WL_CFG80211
extern void wl_cfg80211_set_parent_dev(void *dev);
#endif

extern void sdioh_sdmmc_devintr_off(sdioh_info_t *sd);
extern void sdioh_sdmmc_devintr_on(sdioh_info_t *sd);
extern int dhd_os_check_wakelock(void *dhdp);
extern int dhd_os_check_if_up(void *dhdp);
extern void *bcmsdh_get_drvdata(void);
=======
extern void wl_cfg80211_set_parent_dev(void *dev);
extern void sdioh_sdmmc_devintr_off(sdioh_info_t *sd);
extern void sdioh_sdmmc_devintr_on(sdioh_info_t *sd);
extern void* bcmsdh_probe(osl_t *osh, void *dev, void *sdioh, void *adapter_info, uint bus_type,
	uint bus_num, uint slot_num);
extern int bcmsdh_remove(bcmsdh_info_t *bcmsdh);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

int sdio_function_init(void);
void sdio_function_cleanup(void);

#define DESCRIPTION "bcmsdh_sdmmc Driver"
#define AUTHOR "Broadcom Corporation"

/* module param defaults */
static int clockoverride = 0;

module_param(clockoverride, int, 0644);
MODULE_PARM_DESC(clockoverride, "SDIO card clock override");

<<<<<<< HEAD
PBCMSDH_SDMMC_INSTANCE gInstance;

/* Maximum number of bcmsdh_sdmmc devices supported by driver */
#define BCMSDH_SDMMC_MAX_DEVICES 1

extern int bcmsdh_probe(struct device *dev);
extern int bcmsdh_remove(struct device *dev);
extern volatile bool dhd_mmc_suspend;

=======
/* Maximum number of bcmsdh_sdmmc devices supported by driver */
#define BCMSDH_SDMMC_MAX_DEVICES 1

extern volatile bool dhd_mmc_suspend;

static int sdioh_probe(struct sdio_func *func)
{
	int host_idx = func->card->host->index;
	uint32 rca = func->card->rca;
	wifi_adapter_info_t *adapter;
	osl_t *osh = NULL;
	sdioh_info_t *sdioh = NULL;

	sd_err(("bus num (host idx)=%d, slot num (rca)=%d\n", host_idx, rca));
	adapter = dhd_wifi_platform_get_adapter(SDIO_BUS, host_idx, rca);
	if (adapter  != NULL)
		sd_err(("found adapter info '%s'\n", adapter->name));
	else
		sd_err(("can't find adapter info for this chip\n"));

#ifdef WL_CFG80211
	wl_cfg80211_set_parent_dev(&func->dev);
#endif

	 /* allocate SDIO Host Controller state info */
	 osh = osl_attach(&func->dev, SDIO_BUS, TRUE);
	 if (osh == NULL) {
		 sd_err(("%s: osl_attach failed\n", __FUNCTION__));
		 goto fail;
	 }
	 osl_static_mem_init(osh, adapter);
	 sdioh = sdioh_attach(osh, func);
	 if (sdioh == NULL) {
		 sd_err(("%s: sdioh_attach failed\n", __FUNCTION__));
		 goto fail;
	 }
	 sdioh->bcmsdh = bcmsdh_probe(osh, &func->dev, sdioh, adapter, SDIO_BUS, host_idx, rca);
	 if (sdioh->bcmsdh == NULL) {
		 sd_err(("%s: bcmsdh_probe failed\n", __FUNCTION__));
		 goto fail;
	 }

	sdio_set_drvdata(func, sdioh);
	return 0;

fail:
	if (sdioh != NULL)
		sdioh_detach(osh, sdioh);
	if (osh != NULL)
		osl_detach(osh);
	return -ENOMEM;
}

static void sdioh_remove(struct sdio_func *func)
{
	sdioh_info_t *sdioh;
	osl_t *osh;

	sdioh = sdio_get_drvdata(func);
	if (sdioh == NULL) {
		sd_err(("%s: error, no sdioh handler found\n", __FUNCTION__));
		return;
	}

	osh = sdioh->osh;
	bcmsdh_remove(sdioh->bcmsdh);
	sdioh_detach(osh, sdioh);
	osl_detach(osh);
}

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
static int bcmsdh_sdmmc_probe(struct sdio_func *func,
                              const struct sdio_device_id *id)
{
	int ret = 0;
<<<<<<< HEAD
	static struct sdio_func sdio_func_0;

	if (!gInstance)
		return -EINVAL;

	if (func) {
		sd_trace(("bcmsdh_sdmmc: %s Enter\n", __FUNCTION__));
		sd_trace(("sdio_bcmsdh: func->class=%x\n", func->class));
		sd_trace(("sdio_vendor: 0x%04x\n", func->vendor));
		sd_trace(("sdio_device: 0x%04x\n", func->device));
		sd_trace(("Function#: 0x%04x\n", func->num));

		if (func->num == 1) {
			sdio_func_0.num = 0;
			sdio_func_0.card = func->card;
			gInstance->func[0] = &sdio_func_0;
			if(func->device == 0x4) { /* 4318 */
				gInstance->func[2] = NULL;
				sd_trace(("NIC found, calling bcmsdh_probe...\n"));
				ret = bcmsdh_probe(&func->dev);
			}
		}

		gInstance->func[func->num] = func;

		if (func->num == 2) {
	#ifdef WL_CFG80211
			wl_cfg80211_set_parent_dev(&func->dev);
	#endif
			sd_trace(("F2 found, calling bcmsdh_probe...\n"));
			ret = bcmsdh_probe(&func->dev);
			if (ret < 0)
				gInstance->func[2] = NULL;
		}
	} else {
		ret = -ENODEV;
	}
=======

	if (func == NULL)
		return -EINVAL;

	sd_err(("bcmsdh_sdmmc: %s Enter\n", __FUNCTION__));
	sd_info(("sdio_bcmsdh: func->class=%x\n", func->class));
	sd_info(("sdio_vendor: 0x%04x\n", func->vendor));
	sd_info(("sdio_device: 0x%04x\n", func->device));
	sd_info(("Function#: 0x%04x\n", func->num));

	/* 4318 doesn't have function 2 */
	if ((func->num == 2) || (func->num == 1 && func->device == 0x4))
		ret = sdioh_probe(func);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

	return ret;
}

static void bcmsdh_sdmmc_remove(struct sdio_func *func)
{
<<<<<<< HEAD
	if (func) {
		sd_trace(("bcmsdh_sdmmc: %s Enter\n", __FUNCTION__));
		sd_info(("sdio_bcmsdh: func->class=%x\n", func->class));
		sd_info(("sdio_vendor: 0x%04x\n", func->vendor));
		sd_info(("sdio_device: 0x%04x\n", func->device));
		sd_info(("Function#: 0x%04x\n", func->num));

		if (gInstance->func[2]) {
			sd_trace(("F2 found, calling bcmsdh_remove...\n"));
			bcmsdh_remove(&func->dev);
			gInstance->func[2] = NULL;
		}
		if (func->num == 1) {
			sdio_claim_host(func);
			sdio_disable_func(func);
			sdio_release_host(func);
			gInstance->func[1] = NULL;
		}
	}
=======
	if (func == NULL) {
		sd_err(("%s is called with NULL SDIO function pointer\n", __FUNCTION__));
		return;
	}

	sd_trace(("bcmsdh_sdmmc: %s Enter\n", __FUNCTION__));
	sd_info(("sdio_bcmsdh: func->class=%x\n", func->class));
	sd_info(("sdio_vendor: 0x%04x\n", func->vendor));
	sd_info(("sdio_device: 0x%04x\n", func->device));
	sd_info(("Function#: 0x%04x\n", func->num));

	if ((func->num == 2) || (func->num == 1 && func->device == 0x4))
		sdioh_remove(func);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
}

/* devices we support, null terminated */
static const struct sdio_device_id bcmsdh_sdmmc_ids[] = {
	{ SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_DEFAULT) },
	{ SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_4325_SDGWB) },
	{ SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_4325) },
	{ SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_4329) },
	{ SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_4319) },
	{ SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_4330) },
	{ SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_4334) },
	{ SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_4324) },
	{ SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_43239) },
	{ SDIO_DEVICE_CLASS(SDIO_CLASS_NONE)		},
	{ /* end: all zeroes */				},
};

MODULE_DEVICE_TABLE(sdio, bcmsdh_sdmmc_ids);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 39)) && defined(CONFIG_PM)
static int bcmsdh_sdmmc_suspend(struct device *pdev)
{
<<<<<<< HEAD
	struct sdio_func *func = dev_to_sdio_func(pdev);
	mmc_pm_flag_t sdio_flags;
	int ret;

	if (func->num != 2)
		return 0;

#ifdef CUSTOMER_HW4
	sd_err(("%s Enter\n", __FUNCTION__));
#else
	sd_trace(("%s Enter\n", __FUNCTION__));
#endif
	if (dhd_os_check_wakelock(bcmsdh_get_drvdata()))
		return -EBUSY;
	sdio_flags = sdio_get_host_pm_caps(func);

	if (!(sdio_flags & MMC_PM_KEEP_POWER)) {
		sd_err(("%s: can't keep power while host is suspended\n", __FUNCTION__));
=======
	int err;
	sdioh_info_t *sdioh;
	struct sdio_func *func = dev_to_sdio_func(pdev);
	mmc_pm_flag_t sdio_flags;

	sd_err(("%s Enter\n", __FUNCTION__));
	if (func->num != 2)
		return 0;

	dhd_mmc_suspend = TRUE;
	sdioh = sdio_get_drvdata(func);
	err = bcmsdh_suspend(sdioh->bcmsdh);
	if (err) {
		dhd_mmc_suspend = FALSE;
		return err;
	}

	sdio_flags = sdio_get_host_pm_caps(func);
	if (!(sdio_flags & MMC_PM_KEEP_POWER)) {
		sd_err(("%s: can't keep power while host is suspended\n", __FUNCTION__));
		dhd_mmc_suspend = FALSE;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		return  -EINVAL;
	}

	/* keep power while host suspended */
<<<<<<< HEAD
	ret = sdio_set_host_pm_flags(func, MMC_PM_KEEP_POWER);
	if (ret) {
		sd_err(("%s: error while trying to keep power\n", __FUNCTION__));
		return ret;
	}
#if defined(OOB_INTR_ONLY) && !defined(CUSTOMER_HW4)
	bcmsdh_oob_intr_set(0);
#endif /* OOB_INTR_ONLY && !CUSTOMER_HW4 */
	dhd_mmc_suspend = TRUE;
=======
	err = sdio_set_host_pm_flags(func, MMC_PM_KEEP_POWER);
	if (err) {
		sd_err(("%s: error while trying to keep power\n", __FUNCTION__));
		dhd_mmc_suspend = FALSE;
		return err;
	}
#if defined(OOB_INTR_ONLY) && !defined(CUSTOMER_HW4)
	bcmsdh_oob_intr_set(sdioh->bcmsdh, FALSE);
#endif /* OOB_INTR_ONLY && !CUSTOMER_HW4 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	smp_mb();

	return 0;
}

static int bcmsdh_sdmmc_resume(struct device *pdev)
{
<<<<<<< HEAD
#if defined(OOB_INTR_ONLY) && !defined(CUSTOMER_HW4)
	struct sdio_func *func = dev_to_sdio_func(pdev);
#endif /* OOB_INTR_ONLY && !CUSTOMER_HW4 */
#ifdef CUSTOMER_HW4
	sd_err(("%s Enter\n", __FUNCTION__));
#else
	sd_trace(("%s Enter\n", __FUNCTION__));
#endif
	dhd_mmc_suspend = FALSE;
#if defined(OOB_INTR_ONLY) && !defined(CUSTOMER_HW4)
	if ((func->num == 2) && dhd_os_check_if_up(bcmsdh_get_drvdata()))
		bcmsdh_oob_intr_set(1);
=======
	sdioh_info_t *sdioh;
	struct sdio_func *func = dev_to_sdio_func(pdev);

	sd_err(("%s Enter\n", __FUNCTION__));
	if (func->num != 2)
		return 0;

	sdioh = sdio_get_drvdata(func);
	dhd_mmc_suspend = FALSE;
#if defined(OOB_INTR_ONLY) && !defined(CUSTOMER_HW4)
	bcmsdh_resume(sdioh->bcmsdh);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#endif /* OOB_INTR_ONLY && !CUSTOMER_HW4 */

	smp_mb();
	return 0;
}

static const struct dev_pm_ops bcmsdh_sdmmc_pm_ops = {
	.suspend	= bcmsdh_sdmmc_suspend,
	.resume		= bcmsdh_sdmmc_resume,
};
#endif  /* (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 39)) && defined(CONFIG_PM) */

#if defined(BCMLXSDMMC)
static struct semaphore *notify_semaphore = NULL;

static int dummy_probe(struct sdio_func *func,
                              const struct sdio_device_id *id)
{
	if (func && (func->num != 2)) {
		return 0;
	}

	if (notify_semaphore)
		up(notify_semaphore);
	return 0;
}

static void dummy_remove(struct sdio_func *func)
{
}

static struct sdio_driver dummy_sdmmc_driver = {
	.probe		= dummy_probe,
	.remove		= dummy_remove,
	.name		= "dummy_sdmmc",
	.id_table	= bcmsdh_sdmmc_ids,
	};

int sdio_func_reg_notify(void* semaphore)
{
	notify_semaphore = semaphore;
	return sdio_register_driver(&dummy_sdmmc_driver);
}

void sdio_func_unreg_notify(void)
{
	OSL_SLEEP(15);
	sdio_unregister_driver(&dummy_sdmmc_driver);
}

#endif /* defined(BCMLXSDMMC) */

static struct sdio_driver bcmsdh_sdmmc_driver = {
	.probe		= bcmsdh_sdmmc_probe,
	.remove		= bcmsdh_sdmmc_remove,
	.name		= "bcmsdh_sdmmc",
	.id_table	= bcmsdh_sdmmc_ids,
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 39)) && defined(CONFIG_PM)
	.drv = {
	.pm	= &bcmsdh_sdmmc_pm_ops,
	},
#endif /* (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 39)) && defined(CONFIG_PM) */
	};

struct sdos_info {
	sdioh_info_t *sd;
	spinlock_t lock;
};

<<<<<<< HEAD

int
sdioh_sdmmc_osinit(sdioh_info_t *sd)
{
	struct sdos_info *sdos;

	if (!sd)
		return BCME_BADARG;

	sdos = (struct sdos_info*)MALLOC(sd->osh, sizeof(struct sdos_info));
	sd->sdos_info = (void*)sdos;
	if (sdos == NULL)
		return BCME_NOMEM;

	sdos->sd = sd;
	spin_lock_init(&sdos->lock);
	return BCME_OK;
}

void
sdioh_sdmmc_osfree(sdioh_info_t *sd)
{
	struct sdos_info *sdos;
	ASSERT(sd && sd->sdos_info);

	sdos = (struct sdos_info *)sd->sdos_info;
	MFREE(sd->osh, sdos, sizeof(struct sdos_info));
}

=======
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
/* Interrupt enable/disable */
SDIOH_API_RC
sdioh_interrupt_set(sdioh_info_t *sd, bool enable)
{
<<<<<<< HEAD
	ulong flags;
	struct sdos_info *sdos;

=======
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	if (!sd)
		return BCME_BADARG;

	sd_trace(("%s: %s\n", __FUNCTION__, enable ? "Enabling" : "Disabling"));
<<<<<<< HEAD

	sdos = (struct sdos_info *)sd->sdos_info;
	ASSERT(sdos);

#if !defined(OOB_INTR_ONLY)
	if (enable && !(sd->intr_handler && sd->intr_handler_arg)) {
		sd_err(("%s: no handler registered, will not enable\n", __FUNCTION__));
		return SDIOH_API_RC_FAIL;
	}
#endif /* !defined(OOB_INTR_ONLY) */

	/* Ensure atomicity for enable/disable calls */
	spin_lock_irqsave(&sdos->lock, flags);

	sd->client_intr_enabled = enable;
	if (enable) {
		sdioh_sdmmc_devintr_on(sd);
	} else {
		sdioh_sdmmc_devintr_off(sd);
	}

	spin_unlock_irqrestore(&sdos->lock, flags);

	return SDIOH_API_RC_SUCCESS;
}


=======
	return SDIOH_API_RC_SUCCESS;
}

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#ifdef BCMSDH_MODULE
static int __init
bcmsdh_module_init(void)
{
	int error = 0;
	error = sdio_function_init();
	return error;
}

static void __exit
bcmsdh_module_cleanup(void)
{
	sdio_function_cleanup();
}

module_init(bcmsdh_module_init);
module_exit(bcmsdh_module_cleanup);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_AUTHOR(AUTHOR);

#endif /* BCMSDH_MODULE */
/*
 * module init
*/
<<<<<<< HEAD
int sdio_function_init(void)
{
	int error = 0;
	sd_trace(("bcmsdh_sdmmc: %s Enter\n", __FUNCTION__));

	gInstance = kzalloc(sizeof(BCMSDH_SDMMC_INSTANCE), GFP_KERNEL);
	if (!gInstance)
		return -ENOMEM;

	error = sdio_register_driver(&bcmsdh_sdmmc_driver);
	if (error) {
		kfree(gInstance);
		gInstance = NULL;
	}

	return error;
=======
int bcmsdh_register_client_driver(void)
{
	return sdio_register_driver(&bcmsdh_sdmmc_driver);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
}

/*
 * module cleanup
*/
<<<<<<< HEAD
extern int bcmsdh_remove(struct device *dev);
void sdio_function_cleanup(void)
{
	sd_trace(("%s Enter\n", __FUNCTION__));


	sdio_unregister_driver(&bcmsdh_sdmmc_driver);

	if (gInstance) {
		kfree(gInstance);
		gInstance = NULL;
	}
=======
void bcmsdh_unregister_client_driver(void)
{
	sdio_unregister_driver(&bcmsdh_sdmmc_driver);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
}
