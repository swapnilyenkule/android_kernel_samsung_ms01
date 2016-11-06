/*
 * HND SiliconBackplane PMU support.
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
 * $Id: hndpmu.h 385540 2013-02-15 23:14:50Z $
=======
 * $Id: hndpmu.h 431134 2013-10-22 18:25:42Z $
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 */

#ifndef _hndpmu_h_
#define _hndpmu_h_


<<<<<<< HEAD
extern void si_pmu_otp_power(si_t *sih, osl_t *osh, bool on);
=======
extern void si_pmu_otp_power(si_t *sih, osl_t *osh, bool on, uint32* min_res_mask);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
extern void si_sdiod_drive_strength_init(si_t *sih, osl_t *osh, uint32 drivestrength);

extern void si_pmu_minresmask_htavail_set(si_t *sih, osl_t *osh, bool set_clear);

#endif /* _hndpmu_h_ */
