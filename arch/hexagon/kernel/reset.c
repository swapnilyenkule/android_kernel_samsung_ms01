/*
<<<<<<< HEAD
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <linux/smp.h>
#include <asm/hexagon_vm.h>

void machine_power_off(void)
{
	smp_send_stop();
	__vmstop();
}

void machine_halt(void)
{
}

void machine_restart(char *cmd)
{
}

void pm_power_off(void)
{
}
