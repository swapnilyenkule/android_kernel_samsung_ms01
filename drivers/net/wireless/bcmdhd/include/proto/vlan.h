/*
 * 802.1Q VLAN protocol definitions
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
 * $Id: vlan.h 382883 2013-02-04 23:26:09Z $
 */

#ifndef _vlan_h_
#define _vlan_h_

#ifndef _TYPEDEFS_H_
#include <typedefs.h>
#endif

<<<<<<< HEAD

#include <packed_section_start.h>

#ifndef	 VLAN_VID_MASK
#define VLAN_VID_MASK		0xfff	
#endif

#define	VLAN_CFI_SHIFT		12	
#define VLAN_PRI_SHIFT		13	

#define VLAN_PRI_MASK		7	

#define	VLAN_TPID_OFFSET	12	
#define	VLAN_TCI_OFFSET		14	

#define	VLAN_TAG_LEN		4
#define	VLAN_TAG_OFFSET		(2 * ETHER_ADDR_LEN)	

#define VLAN_TPID		0x8100	

struct vlan_header {
	uint16	vlan_type;		
	uint16	vlan_tag;		
=======
/* This marks the start of a packed structure section. */
#include <packed_section_start.h>

#ifndef	 VLAN_VID_MASK
#define VLAN_VID_MASK		0xfff	/* low 12 bits are vlan id */
#endif

#define	VLAN_CFI_SHIFT		12	/* canonical format indicator bit */
#define VLAN_PRI_SHIFT		13	/* user priority */

#define VLAN_PRI_MASK		7	/* 3 bits of priority */

#define	VLAN_TPID_OFFSET	12	/* offset of tag protocol id field */
#define	VLAN_TCI_OFFSET		14	/* offset of tag ctrl info field */

#define	VLAN_TAG_LEN		4
#define	VLAN_TAG_OFFSET		(2 * ETHER_ADDR_LEN)	/* offset in Ethernet II packet only */

#define VLAN_TPID		0x8100	/* VLAN ethertype/Tag Protocol ID */

struct vlan_header {
	uint16	vlan_type;		/* 0x8100 */
	uint16	vlan_tag;		/* priority, cfi and vid */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
};

struct ethervlan_header {
	uint8	ether_dhost[ETHER_ADDR_LEN];
	uint8	ether_shost[ETHER_ADDR_LEN];
<<<<<<< HEAD
	uint16	vlan_type;		
	uint16	vlan_tag;		
=======
	uint16	vlan_type;		/* 0x8100 */
	uint16	vlan_tag;		/* priority, cfi and vid */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	uint16	ether_type;
};

struct dot3_mac_llc_snapvlan_header {
<<<<<<< HEAD
	uint8	ether_dhost[ETHER_ADDR_LEN];	
	uint8	ether_shost[ETHER_ADDR_LEN];	
	uint16	length;				
	uint8	dsap;				
	uint8	ssap;				
	uint8	ctl;				
	uint8	oui[3];				
	uint16	vlan_type;			
	uint16	vlan_tag;			
	uint16	ether_type;			
=======
	uint8	ether_dhost[ETHER_ADDR_LEN];	/* dest mac */
	uint8	ether_shost[ETHER_ADDR_LEN];	/* src mac */
	uint16	length;				/* frame length incl header */
	uint8	dsap;				/* always 0xAA */
	uint8	ssap;				/* always 0xAA */
	uint8	ctl;				/* always 0x03 */
	uint8	oui[3];				/* RFC1042: 0x00 0x00 0x00
						 * Bridge-Tunnel: 0x00 0x00 0xF8
						 */
	uint16	vlan_type;			/* 0x8100 */
	uint16	vlan_tag;			/* priority, cfi and vid */
	uint16	ether_type;			/* ethertype */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
};

#define	ETHERVLAN_HDR_LEN	(ETHER_HDR_LEN + VLAN_TAG_LEN)


<<<<<<< HEAD

=======
/* This marks the end of a packed structure section. */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#include <packed_section_end.h>

#define ETHERVLAN_MOVE_HDR(d, s) \
do { \
	struct ethervlan_header t; \
	t = *(struct ethervlan_header *)(s); \
	*(struct ethervlan_header *)(d) = t; \
} while (0)

<<<<<<< HEAD
#endif 
=======
#endif /* _vlan_h_ */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
