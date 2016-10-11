/*
 * Copyright (C) 2007 Google, Inc.
<<<<<<< HEAD
 * Copyright (c) 2007-2011, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2007-2010, Code Aurora Forum. All rights reserved.
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/err.h>

#include <mach/clk.h>
#include <mach/clk-provider.h>
#include <mach/socinfo.h>
#include <mach/proc_comm.h>

=======
#include <linux/err.h>
#include <linux/ctype.h>
#include <linux/stddef.h>
#include <mach/clk.h>

#include "proc_comm.h"
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
#include "clock.h"
#include "clock-pcom.h"

/*
 * glue for the proc_comm interface
 */
<<<<<<< HEAD
static int pc_clk_enable(struct clk *clk)
{
	int rc;
	int id = to_pcom_clk(clk)->id;

	/* Ignore clocks that are always on */
	if (id == P_EBI1_CLK || id == P_EBI1_FIXED_CLK)
		return 0;

	rc = msm_proc_comm(PCOM_CLKCTL_RPC_ENABLE, &id, NULL);
=======
int pc_clk_enable(unsigned id)
{
	int rc = msm_proc_comm(PCOM_CLKCTL_RPC_ENABLE, &id, NULL);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	if (rc < 0)
		return rc;
	else
		return (int)id < 0 ? -EINVAL : 0;
}

<<<<<<< HEAD
static void pc_clk_disable(struct clk *clk)
{
	int id = to_pcom_clk(clk)->id;

	/* Ignore clocks that are always on */
	if (id == P_EBI1_CLK || id == P_EBI1_FIXED_CLK)
		return;

=======
void pc_clk_disable(unsigned id)
{
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	msm_proc_comm(PCOM_CLKCTL_RPC_DISABLE, &id, NULL);
}

int pc_clk_reset(unsigned id, enum clk_reset_action action)
{
	int rc;

	if (action == CLK_RESET_ASSERT)
		rc = msm_proc_comm(PCOM_CLKCTL_RPC_RESET_ASSERT, &id, NULL);
	else
		rc = msm_proc_comm(PCOM_CLKCTL_RPC_RESET_DEASSERT, &id, NULL);

	if (rc < 0)
		return rc;
	else
		return (int)id < 0 ? -EINVAL : 0;
}

<<<<<<< HEAD
static int pc_reset(struct clk *clk, enum clk_reset_action action)
{
	int id = to_pcom_clk(clk)->id;
	return pc_clk_reset(id, action);
}

static int _pc_clk_set_rate(struct clk *clk, unsigned long rate)
=======
int pc_clk_set_rate(unsigned id, unsigned rate)
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
{
	/* The rate _might_ be rounded off to the nearest KHz value by the
	 * remote function. So a return value of 0 doesn't necessarily mean
	 * that the exact rate was set successfully.
	 */
<<<<<<< HEAD
	unsigned r = rate;
	int id = to_pcom_clk(clk)->id;
	int rc = msm_proc_comm(PCOM_CLKCTL_RPC_SET_RATE, &id, &r);
=======
	int rc = msm_proc_comm(PCOM_CLKCTL_RPC_SET_RATE, &id, &rate);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	if (rc < 0)
		return rc;
	else
		return (int)id < 0 ? -EINVAL : 0;
}

<<<<<<< HEAD
static int _pc_clk_set_min_rate(struct clk *clk, unsigned long rate)
{
	int rc;
	int id = to_pcom_clk(clk)->id;
	bool ignore_error = (cpu_is_msm7x27() && id == P_EBI1_CLK &&
				rate >= INT_MAX);
	unsigned r = rate;
	rc = msm_proc_comm(PCOM_CLKCTL_RPC_MIN_RATE, &id, &r);
	if (rc < 0)
		return rc;
	else if (ignore_error)
		return 0;
=======
int pc_clk_set_min_rate(unsigned id, unsigned rate)
{
	int rc = msm_proc_comm(PCOM_CLKCTL_RPC_MIN_RATE, &id, &rate);
	if (rc < 0)
		return rc;
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	else
		return (int)id < 0 ? -EINVAL : 0;
}

<<<<<<< HEAD
static int pc_clk_set_rate(struct clk *clk, unsigned long rate)
{
	if (clk->flags & CLKFLAG_MIN)
		return _pc_clk_set_min_rate(clk, rate);
	else
		return _pc_clk_set_rate(clk, rate);
}

static int pc_clk_set_max_rate(struct clk *clk, unsigned long rate)
{
	int id = to_pcom_clk(clk)->id;
	unsigned r = rate;
	int rc = msm_proc_comm(PCOM_CLKCTL_RPC_MAX_RATE, &id, &r);
=======
int pc_clk_set_max_rate(unsigned id, unsigned rate)
{
	int rc = msm_proc_comm(PCOM_CLKCTL_RPC_MAX_RATE, &id, &rate);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	if (rc < 0)
		return rc;
	else
		return (int)id < 0 ? -EINVAL : 0;
}

<<<<<<< HEAD
static int pc_clk_set_flags(struct clk *clk, unsigned flags)
{
	int id = to_pcom_clk(clk)->id;
=======
int pc_clk_set_flags(unsigned id, unsigned flags)
{
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	int rc = msm_proc_comm(PCOM_CLKCTL_RPC_SET_FLAGS, &id, &flags);
	if (rc < 0)
		return rc;
	else
		return (int)id < 0 ? -EINVAL : 0;
}

<<<<<<< HEAD
static int pc_clk_set_ext_config(struct clk *clk, unsigned long config)
{
	int id = to_pcom_clk(clk)->id;
	unsigned c = config;
	int rc = msm_proc_comm(PCOM_CLKCTL_RPC_SET_EXT_CONFIG, &id, &c);
	if (rc < 0)
		return rc;
	else
		return (int)id < 0 ? -EINVAL : 0;
}

static unsigned long pc_clk_get_rate(struct clk *clk)
{
	int id = to_pcom_clk(clk)->id;
=======
unsigned pc_clk_get_rate(unsigned id)
{
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	if (msm_proc_comm(PCOM_CLKCTL_RPC_RATE, &id, NULL))
		return 0;
	else
		return id;
}

<<<<<<< HEAD
static int pc_clk_is_enabled(struct clk *clk)
{
	int id = to_pcom_clk(clk)->id;
=======
unsigned pc_clk_is_enabled(unsigned id)
{
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	if (msm_proc_comm(PCOM_CLKCTL_RPC_ENABLED, &id, NULL))
		return 0;
	else
		return id;
}

<<<<<<< HEAD
static long pc_clk_round_rate(struct clk *clk, unsigned long rate)
=======
long pc_clk_round_rate(unsigned id, unsigned rate)
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
{

	/* Not really supported; pc_clk_set_rate() does rounding on it's own. */
	return rate;
}

<<<<<<< HEAD
static bool pc_clk_is_local(struct clk *clk)
=======
static bool pc_clk_is_local(unsigned id)
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
{
	return false;
}

<<<<<<< HEAD
static enum handoff pc_clk_handoff(struct clk *clk)
{
	/*
	 * Handoff clock state only since querying and caching the rate here
	 * would incur more overhead than it would ever save.
	 */
	if (pc_clk_is_enabled(clk))
		return HANDOFF_ENABLED_CLK;

	return HANDOFF_DISABLED_CLK;
}

struct clk_ops clk_ops_pcom = {
	.enable = pc_clk_enable,
	.disable = pc_clk_disable,
	.reset = pc_reset,
	.set_rate = pc_clk_set_rate,
=======
struct clk_ops clk_ops_pcom = {
	.enable = pc_clk_enable,
	.disable = pc_clk_disable,
	.auto_off = pc_clk_disable,
	.reset = pc_clk_reset,
	.set_rate = pc_clk_set_rate,
	.set_min_rate = pc_clk_set_min_rate,
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	.set_max_rate = pc_clk_set_max_rate,
	.set_flags = pc_clk_set_flags,
	.get_rate = pc_clk_get_rate,
	.is_enabled = pc_clk_is_enabled,
	.round_rate = pc_clk_round_rate,
	.is_local = pc_clk_is_local,
<<<<<<< HEAD
	.handoff = pc_clk_handoff,
};

struct clk_ops clk_ops_pcom_ext_config = {
	.enable = pc_clk_enable,
	.disable = pc_clk_disable,
	.reset = pc_reset,
	.set_rate = pc_clk_set_ext_config,
	.set_max_rate = pc_clk_set_max_rate,
	.set_flags = pc_clk_set_flags,
	.get_rate = pc_clk_get_rate,
	.is_enabled = pc_clk_is_enabled,
	.round_rate = pc_clk_round_rate,
	.is_local = pc_clk_is_local,
	.handoff = pc_clk_handoff,
};

=======
};
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
