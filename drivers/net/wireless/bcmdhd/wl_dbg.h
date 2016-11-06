/*
 * Minimal debug/trace/assert driver definitions for
 * Broadcom 802.11 Networking Adapter.
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
 * $Id: wl_dbg.h 376019 2012-12-21 01:00:06Z $
=======
 * $Id: wl_dbg.h 430628 2013-10-19 04:07:25Z $
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 */


#ifndef _wl_dbg_h_
#define _wl_dbg_h_

<<<<<<< HEAD

=======
/* wl_msg_level is a bit vector with defs in wlioctl.h */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
extern uint32 wl_msg_level;
extern uint32 wl_msg_level2;

#define WL_TIMESTAMP()

<<<<<<< HEAD
#if 0 && (VERSION_MAJOR > 9)
#include <IOKit/apple80211/IO8Log.h>
#define WL_PRINT(args)		do { printf args; IO8Log args; } while (0)
#else
#define WL_PRINT(args)		do { WL_TIMESTAMP(); printf args; } while (0)
#endif



=======
#define WL_PRINT(args)		do { WL_TIMESTAMP(); printf args; } while (0)

#if defined(EVENT_LOG_COMPILE) && defined(WLMSG_SRSCAN)
#define _WL_SRSCAN(fmt, ...)	EVENT_LOG(EVENT_LOG_TAG_SRSCAN, fmt, ##__VA_ARGS__)
#define WL_SRSCAN(args)		_WL_SRSCAN args
#else
#define WL_SRSCAN(args)
#endif


/* To disable a message completely ... until you need it again */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#define WL_NONE(args)

#define	WL_ERROR(args)
#define	WL_TRACE(args)
#define WL_APSTA_UPDN(args)
#define WL_APSTA_RX(args)
#ifdef WLMSG_WSEC
#define WL_WSEC(args)		WL_PRINT(args)
#define WL_WSEC_DUMP(args)	WL_PRINT(args)
#else
#define WL_WSEC(args)
#define WL_WSEC_DUMP(args)
#endif
<<<<<<< HEAD

extern uint32 wl_msg_level;
extern uint32 wl_msg_level2;
#endif 
=======
#define WL_PCIE(args)		do {if (wl_msg_level2 & WL_PCIE_VAL) WL_PRINT(args);} while (0)
#define WL_PCIE_ON()		(wl_msg_level2 & WL_PCIE_VAL)

extern uint32 wl_msg_level;
extern uint32 wl_msg_level2;
#endif /* _wl_dbg_h_ */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
