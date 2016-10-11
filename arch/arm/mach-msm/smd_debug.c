/* arch/arm/mach-msm/smd_debug.c
 *
 * Copyright (C) 2007 Google, Inc.
<<<<<<< HEAD
 * Copyright (c) 2009-2013, The Linux Foundation. All rights reserved.
=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
 * Author: Brian Swetland <swetland@google.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/debugfs.h>
#include <linux/list.h>
<<<<<<< HEAD
#include <linux/ctype.h>
#include <linux/jiffies.h>

#include <mach/msm_iomap.h>
#include <mach/msm_smem.h>

#include "smd_private.h"
#include "smem_private.h"
=======

#include <mach/msm_iomap.h>

#include "smd_private.h"
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4

#if defined(CONFIG_DEBUG_FS)

static char *chstate(unsigned n)
{
	switch (n) {
	case SMD_SS_CLOSED:
		return "CLOSED";
	case SMD_SS_OPENING:
		return "OPENING";
	case SMD_SS_OPENED:
		return "OPENED";
	case SMD_SS_FLUSHING:
		return "FLUSHING";
	case SMD_SS_CLOSING:
		return "CLOSING";
	case SMD_SS_RESET:
		return "RESET";
	case SMD_SS_RESET_OPENING:
		return "ROPENING";
	default:
		return "UNKNOWN";
	}
}

<<<<<<< HEAD
static int debug_f3(char *buf, int max)
{
	char *x;
	int size;
	int i = 0, j = 0;
	unsigned cols = 0;
	char str[4*sizeof(unsigned)+1] = {0};

	i += scnprintf(buf + i, max - i,
		       "Printing to log\n");

	x = smem_get_entry(SMEM_ERR_F3_TRACE_LOG, &size);
	if (x != 0) {
		pr_info("smem: F3 TRACE LOG\n");
		while (size > 0) {
			if (size >= sizeof(unsigned)) {
				pr_info("%08x", *((unsigned *) x));
				for (j = 0; j < sizeof(unsigned); ++j)
					if (isprint(*(x+j)))
						str[cols*sizeof(unsigned) + j]
							= *(x+j);
					else
						str[cols*sizeof(unsigned) + j]
							= '-';
				x += sizeof(unsigned);
				size -= sizeof(unsigned);
			} else {
				while (size-- > 0)
					pr_info("%02x", (unsigned) *x++);
				break;
			}
			if (cols == 3) {
				cols = 0;
				str[4*sizeof(unsigned)] = 0;
				pr_info(" %s\n", str);
				str[0] = 0;
			} else {
				cols++;
				pr_info(" ");
			}
		}
		pr_info("\n");
	}

	return max;
}

static int debug_int_stats(char *buf, int max)
{
	int i = 0;
	int subsys;
	struct interrupt_stat *stats = interrupt_stats;
	const char *subsys_name;

	i += scnprintf(buf + i, max - i,
		"   Subsystem    | Interrupt ID |    In     | Out (Hardcoded) |"
		" Out (Configured)|\n");

	for (subsys = 0; subsys < NUM_SMD_SUBSYSTEMS; ++subsys) {
		subsys_name = smd_pid_to_subsystem(subsys);
		if (subsys_name) {
			i += scnprintf(buf + i, max - i,
				"%-10s %4s |    %9d | %9u |       %9u |       %9u |\n",
				smd_pid_to_subsystem(subsys), "smd",
				stats->smd_interrupt_id,
				stats->smd_in_count,
				stats->smd_out_hardcode_count,
				stats->smd_out_config_count);

			i += scnprintf(buf + i, max - i,
				"%-10s %4s |    %9d | %9u |       %9u |       %9u |\n",
				smd_pid_to_subsystem(subsys), "smsm",
				stats->smsm_interrupt_id,
				stats->smsm_in_count,
				stats->smsm_out_hardcode_count,
				stats->smsm_out_config_count);
		}
		++stats;
	}

	return i;
}

static int debug_int_stats_reset(char *buf, int max)
{
	int i = 0;
	int subsys;
	struct interrupt_stat *stats = interrupt_stats;

	i += scnprintf(buf + i, max - i, "Resetting interrupt stats.\n");

	for (subsys = 0; subsys < NUM_SMD_SUBSYSTEMS; ++subsys) {
		stats->smd_in_count = 0;
		stats->smd_out_hardcode_count = 0;
		stats->smd_out_config_count = 0;
		stats->smsm_in_count = 0;
		stats->smsm_out_hardcode_count = 0;
		stats->smsm_out_config_count = 0;
		++stats;
	}

	return i;
}

static int debug_diag(char *buf, int max)
{
	int i = 0;

	i += scnprintf(buf + i, max - i,
		       "Printing to log\n");
	smd_diag();

	return i;
}

static int debug_modem_err_f3(char *buf, int max)
{
	char *x;
	int size;
	int i = 0, j = 0;
	unsigned cols = 0;
	char str[4*sizeof(unsigned)+1] = {0};

	x = smem_get_entry(SMEM_ERR_F3_TRACE_LOG, &size);
	if (x != 0) {
		pr_info("smem: F3 TRACE LOG\n");
		while (size > 0 && max - i) {
			if (size >= sizeof(unsigned)) {
				i += scnprintf(buf + i, max - i, "%08x",
					       *((unsigned *) x));
				for (j = 0; j < sizeof(unsigned); ++j)
					if (isprint(*(x+j)))
						str[cols*sizeof(unsigned) + j]
							= *(x+j);
					else
						str[cols*sizeof(unsigned) + j]
							= '-';
				x += sizeof(unsigned);
				size -= sizeof(unsigned);
			} else {
				while (size-- > 0 && max - i)
					i += scnprintf(buf + i, max - i,
						       "%02x",
						       (unsigned) *x++);
				break;
			}
			if (cols == 3) {
				cols = 0;
				str[4*sizeof(unsigned)] = 0;
				i += scnprintf(buf + i, max - i, " %s\n",
					       str);
				str[0] = 0;
			} else {
				cols++;
				i += scnprintf(buf + i, max - i, " ");
			}
		}
		i += scnprintf(buf + i, max - i, "\n");
	}

	return i;
}

static int debug_modem_err(char *buf, int max)
{
	char *x;
	int size;
	int i = 0;

	x = smem_find(ID_DIAG_ERR_MSG, SZ_DIAG_ERR_MSG);
	if (x != 0) {
		x[SZ_DIAG_ERR_MSG - 1] = 0;
		i += scnprintf(buf + i, max - i,
			       "smem: DIAG '%s'\n", x);
	}

	x = smem_get_entry(SMEM_ERR_CRASH_LOG, &size);
	if (x != 0) {
		x[size - 1] = 0;
		i += scnprintf(buf + i, max - i,
			       "smem: CRASH LOG\n'%s'\n", x);
	}
	i += scnprintf(buf + i, max - i, "\n");

	return i;
}

static int debug_read_diag_msg(char *buf, int max)
=======

static int dump_ch(char *buf, int max, struct smd_channel *ch)
{
	volatile struct smd_half_channel *s = ch->send;
	volatile struct smd_half_channel *r = ch->recv;

	return scnprintf(
		buf, max,
		"ch%02d:"
		" %8s(%05d/%05d) %c%c%c%c%c%c%c <->"
		" %8s(%05d/%05d) %c%c%c%c%c%c%c '%s'\n", ch->n,
		chstate(s->state), s->tail, s->head,
		s->fDSR ? 'D' : 'd',
		s->fCTS ? 'C' : 'c',
		s->fCD ? 'C' : 'c',
		s->fRI ? 'I' : 'i',
		s->fHEAD ? 'W' : 'w',
		s->fTAIL ? 'R' : 'r',
		s->fSTATE ? 'S' : 's',
		chstate(r->state), r->tail, r->head,
		r->fDSR ? 'D' : 'd',
		r->fCTS ? 'R' : 'r',
		r->fCD ? 'C' : 'c',
		r->fRI ? 'I' : 'i',
		r->fHEAD ? 'W' : 'w',
		r->fTAIL ? 'R' : 'r',
		r->fSTATE ? 'S' : 's',
		ch->name
		);
}

static int debug_read_stat(char *buf, int max)
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
{
	char *msg;
	int i = 0;

	msg = smem_find(ID_DIAG_ERR_MSG, SZ_DIAG_ERR_MSG);

<<<<<<< HEAD
=======
	if (raw_smsm_get_state(SMSM_STATE_MODEM) & SMSM_RESET)
		i += scnprintf(buf + i, max - i,
			       "smsm: ARM9 HAS CRASHED\n");

	i += scnprintf(buf + i, max - i, "smsm: a9: %08x a11: %08x\n",
		       raw_smsm_get_state(SMSM_STATE_MODEM),
		       raw_smsm_get_state(SMSM_STATE_APPS));
#ifdef CONFIG_ARCH_MSM_SCORPION
	i += scnprintf(buf + i, max - i, "smsm dem: apps: %08x modem: %08x "
		       "qdsp6: %08x power: %08x time: %08x\n",
		       raw_smsm_get_state(SMSM_STATE_APPS_DEM),
		       raw_smsm_get_state(SMSM_STATE_MODEM_DEM),
		       raw_smsm_get_state(SMSM_STATE_QDSP6_DEM),
		       raw_smsm_get_state(SMSM_STATE_POWER_MASTER_DEM),
		       raw_smsm_get_state(SMSM_STATE_TIME_MASTER_DEM));
#endif
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	if (msg) {
		msg[SZ_DIAG_ERR_MSG - 1] = 0;
		i += scnprintf(buf + i, max - i, "diag: '%s'\n", msg);
	}
	return i;
}

<<<<<<< HEAD
static int dump_ch(char *buf, int max, int n,
		   void *half_ch_s,
		   void *half_ch_r,
		   struct smd_half_channel_access *half_ch_funcs,
		   unsigned size)
{
	return scnprintf(
		buf, max,
		"ch%02d:"
		" %8s(%04d/%04d) %c%c%c%c%c%c%c%c <->"
		" %8s(%04d/%04d) %c%c%c%c%c%c%c%c : %5x\n", n,
		chstate(half_ch_funcs->get_state(half_ch_s)),
		half_ch_funcs->get_tail(half_ch_s),
		half_ch_funcs->get_head(half_ch_s),
		half_ch_funcs->get_fDSR(half_ch_s) ? 'D' : 'd',
		half_ch_funcs->get_fCTS(half_ch_s) ? 'C' : 'c',
		half_ch_funcs->get_fCD(half_ch_s) ? 'C' : 'c',
		half_ch_funcs->get_fRI(half_ch_s) ? 'I' : 'i',
		half_ch_funcs->get_fHEAD(half_ch_s) ? 'W' : 'w',
		half_ch_funcs->get_fTAIL(half_ch_s) ? 'R' : 'r',
		half_ch_funcs->get_fSTATE(half_ch_s) ? 'S' : 's',
		half_ch_funcs->get_fBLOCKREADINTR(half_ch_s) ? 'B' : 'b',
		chstate(half_ch_funcs->get_state(half_ch_r)),
		half_ch_funcs->get_tail(half_ch_r),
		half_ch_funcs->get_head(half_ch_r),
		half_ch_funcs->get_fDSR(half_ch_r) ? 'D' : 'd',
		half_ch_funcs->get_fCTS(half_ch_r) ? 'C' : 'c',
		half_ch_funcs->get_fCD(half_ch_r) ? 'C' : 'c',
		half_ch_funcs->get_fRI(half_ch_r) ? 'I' : 'i',
		half_ch_funcs->get_fHEAD(half_ch_r) ? 'W' : 'w',
		half_ch_funcs->get_fTAIL(half_ch_r) ? 'R' : 'r',
		half_ch_funcs->get_fSTATE(half_ch_r) ? 'S' : 's',
		half_ch_funcs->get_fBLOCKREADINTR(half_ch_r) ? 'B' : 'b',
		size
		);
}

static int debug_read_smsm_state(char *buf, int max)
{
	uint32_t *smsm;
	int n, i = 0;

	smsm = smem_find(ID_SHARED_STATE,
			 SMSM_NUM_ENTRIES * sizeof(uint32_t));

	if (smsm)
		for (n = 0; n < SMSM_NUM_ENTRIES; n++)
			i += scnprintf(buf + i, max - i, "entry %d: 0x%08x\n",
				       n, smsm[n]);

	return i;
}

struct SMSM_CB_DATA {
	int cb_count;
	void *data;
	uint32_t old_state;
	uint32_t new_state;
};
static struct SMSM_CB_DATA smsm_cb_data;
static struct completion smsm_cb_completion;

static void smsm_state_cb(void *data, uint32_t old_state, uint32_t new_state)
{
	smsm_cb_data.cb_count++;
	smsm_cb_data.old_state = old_state;
	smsm_cb_data.new_state = new_state;
	smsm_cb_data.data = data;
	complete_all(&smsm_cb_completion);
}

#define UT_EQ_INT(a, b) \
	if ((a) != (b)) { \
		i += scnprintf(buf + i, max - i, \
			"%s:%d " #a "(%d) != " #b "(%d)\n", \
				__func__, __LINE__, \
				a, b); \
		break; \
	} \
	do {} while (0)

#define UT_GT_INT(a, b) \
	if ((a) <= (b)) { \
		i += scnprintf(buf + i, max - i, \
			"%s:%d " #a "(%d) > " #b "(%d)\n", \
				__func__, __LINE__, \
				a, b); \
		break; \
	} \
	do {} while (0)

#define SMSM_CB_TEST_INIT() \
	do { \
		smsm_cb_data.cb_count = 0; \
		smsm_cb_data.old_state = 0; \
		smsm_cb_data.new_state = 0; \
		smsm_cb_data.data = 0; \
	} while (0)


static int debug_test_smsm(char *buf, int max)
{
	int i = 0;
	int test_num = 0;
	int ret;

	/* Test case 1 - Register new callback for notification */
	do {
		test_num++;
		SMSM_CB_TEST_INIT();
		ret = smsm_state_cb_register(SMSM_APPS_STATE, SMSM_SMDINIT,
				smsm_state_cb, (void *)0x1234);
		UT_EQ_INT(ret, 0);

		/* de-assert SMSM_SMD_INIT to trigger state update */
		UT_EQ_INT(smsm_cb_data.cb_count, 0);
		INIT_COMPLETION(smsm_cb_completion);
		smsm_change_state(SMSM_APPS_STATE, SMSM_SMDINIT, 0x0);
		UT_GT_INT((int)wait_for_completion_timeout(&smsm_cb_completion,
					msecs_to_jiffies(20)), 0);

		UT_EQ_INT(smsm_cb_data.cb_count, 1);
		UT_EQ_INT(smsm_cb_data.old_state & SMSM_SMDINIT, SMSM_SMDINIT);
		UT_EQ_INT(smsm_cb_data.new_state & SMSM_SMDINIT, 0x0);
		UT_EQ_INT((int)smsm_cb_data.data, 0x1234);

		/* re-assert SMSM_SMD_INIT to trigger state update */
		INIT_COMPLETION(smsm_cb_completion);
		smsm_change_state(SMSM_APPS_STATE, 0x0, SMSM_SMDINIT);
		UT_GT_INT((int)wait_for_completion_timeout(&smsm_cb_completion,
					msecs_to_jiffies(20)), 0);
		UT_EQ_INT(smsm_cb_data.cb_count, 2);
		UT_EQ_INT(smsm_cb_data.old_state & SMSM_SMDINIT, 0x0);
		UT_EQ_INT(smsm_cb_data.new_state & SMSM_SMDINIT, SMSM_SMDINIT);

		/* deregister callback */
		ret = smsm_state_cb_deregister(SMSM_APPS_STATE, SMSM_SMDINIT,
				smsm_state_cb, (void *)0x1234);
		UT_EQ_INT(ret, 2);

		/* make sure state change doesn't cause any more callbacks */
		INIT_COMPLETION(smsm_cb_completion);
		smsm_change_state(SMSM_APPS_STATE, SMSM_SMDINIT, 0x0);
		smsm_change_state(SMSM_APPS_STATE, 0x0, SMSM_SMDINIT);
		UT_EQ_INT((int)wait_for_completion_timeout(&smsm_cb_completion,
					msecs_to_jiffies(20)), 0);
		UT_EQ_INT(smsm_cb_data.cb_count, 2);

		i += scnprintf(buf + i, max - i, "Test %d - PASS\n", test_num);
	} while (0);

	/* Test case 2 - Update already registered callback */
	do {
		test_num++;
		SMSM_CB_TEST_INIT();
		ret = smsm_state_cb_register(SMSM_APPS_STATE, SMSM_SMDINIT,
				smsm_state_cb, (void *)0x1234);
		UT_EQ_INT(ret, 0);
		ret = smsm_state_cb_register(SMSM_APPS_STATE, SMSM_INIT,
				smsm_state_cb, (void *)0x1234);
		UT_EQ_INT(ret, 1);

		/* verify both callback bits work */
		INIT_COMPLETION(smsm_cb_completion);
		UT_EQ_INT(smsm_cb_data.cb_count, 0);
		smsm_change_state(SMSM_APPS_STATE, SMSM_SMDINIT, 0x0);
		UT_GT_INT((int)wait_for_completion_timeout(&smsm_cb_completion,
					msecs_to_jiffies(20)), 0);
		UT_EQ_INT(smsm_cb_data.cb_count, 1);
		INIT_COMPLETION(smsm_cb_completion);
		smsm_change_state(SMSM_APPS_STATE, 0x0, SMSM_SMDINIT);
		UT_GT_INT((int)wait_for_completion_timeout(&smsm_cb_completion,
					msecs_to_jiffies(20)), 0);
		UT_EQ_INT(smsm_cb_data.cb_count, 2);

		INIT_COMPLETION(smsm_cb_completion);
		smsm_change_state(SMSM_APPS_STATE, SMSM_INIT, 0x0);
		UT_GT_INT((int)wait_for_completion_timeout(&smsm_cb_completion,
					msecs_to_jiffies(20)), 0);
		UT_EQ_INT(smsm_cb_data.cb_count, 3);
		INIT_COMPLETION(smsm_cb_completion);
		smsm_change_state(SMSM_APPS_STATE, 0x0, SMSM_INIT);
		UT_GT_INT((int)wait_for_completion_timeout(&smsm_cb_completion,
					msecs_to_jiffies(20)), 0);
		UT_EQ_INT(smsm_cb_data.cb_count, 4);

		/* deregister 1st callback */
		ret = smsm_state_cb_deregister(SMSM_APPS_STATE, SMSM_SMDINIT,
				smsm_state_cb, (void *)0x1234);
		UT_EQ_INT(ret, 1);
		INIT_COMPLETION(smsm_cb_completion);
		smsm_change_state(SMSM_APPS_STATE, SMSM_SMDINIT, 0x0);
		smsm_change_state(SMSM_APPS_STATE, 0x0, SMSM_SMDINIT);
		UT_EQ_INT((int)wait_for_completion_timeout(&smsm_cb_completion,
					msecs_to_jiffies(20)), 0);
		UT_EQ_INT(smsm_cb_data.cb_count, 4);

		INIT_COMPLETION(smsm_cb_completion);
		smsm_change_state(SMSM_APPS_STATE, SMSM_INIT, 0x0);
		UT_GT_INT((int)wait_for_completion_timeout(&smsm_cb_completion,
					msecs_to_jiffies(20)), 0);
		UT_EQ_INT(smsm_cb_data.cb_count, 5);
		INIT_COMPLETION(smsm_cb_completion);
		smsm_change_state(SMSM_APPS_STATE, 0x0, SMSM_INIT);
		UT_GT_INT((int)wait_for_completion_timeout(&smsm_cb_completion,
					msecs_to_jiffies(20)), 0);
		UT_EQ_INT(smsm_cb_data.cb_count, 6);

		/* deregister 2nd callback */
		ret = smsm_state_cb_deregister(SMSM_APPS_STATE, SMSM_INIT,
				smsm_state_cb, (void *)0x1234);
		UT_EQ_INT(ret, 2);

		/* make sure state change doesn't cause any more callbacks */
		INIT_COMPLETION(smsm_cb_completion);
		smsm_change_state(SMSM_APPS_STATE, SMSM_INIT, 0x0);
		smsm_change_state(SMSM_APPS_STATE, 0x0, SMSM_INIT);
		UT_EQ_INT((int)wait_for_completion_timeout(&smsm_cb_completion,
					msecs_to_jiffies(20)), 0);
		UT_EQ_INT(smsm_cb_data.cb_count, 6);

		i += scnprintf(buf + i, max - i, "Test %d - PASS\n", test_num);
	} while (0);

	/* Test case 3 - Two callback registrations with different data */
	do {
		test_num++;
		SMSM_CB_TEST_INIT();
		ret = smsm_state_cb_register(SMSM_APPS_STATE, SMSM_SMDINIT,
				smsm_state_cb, (void *)0x1234);
		UT_EQ_INT(ret, 0);
		ret = smsm_state_cb_register(SMSM_APPS_STATE, SMSM_INIT,
				smsm_state_cb, (void *)0x3456);
		UT_EQ_INT(ret, 0);

		/* verify both callbacks work */
		INIT_COMPLETION(smsm_cb_completion);
		UT_EQ_INT(smsm_cb_data.cb_count, 0);
		smsm_change_state(SMSM_APPS_STATE, SMSM_SMDINIT, 0x0);
		UT_GT_INT((int)wait_for_completion_timeout(&smsm_cb_completion,
					msecs_to_jiffies(20)), 0);
		UT_EQ_INT(smsm_cb_data.cb_count, 1);
		UT_EQ_INT((int)smsm_cb_data.data, 0x1234);

		INIT_COMPLETION(smsm_cb_completion);
		smsm_change_state(SMSM_APPS_STATE, SMSM_INIT, 0x0);
		UT_GT_INT((int)wait_for_completion_timeout(&smsm_cb_completion,
					msecs_to_jiffies(20)), 0);
		UT_EQ_INT(smsm_cb_data.cb_count, 2);
		UT_EQ_INT((int)smsm_cb_data.data, 0x3456);

		/* cleanup and unregister
		 * degregister in reverse to verify data field is
		 * being used
		 */
		smsm_change_state(SMSM_APPS_STATE, 0x0, SMSM_SMDINIT);
		smsm_change_state(SMSM_APPS_STATE, 0x0, SMSM_INIT);
		ret = smsm_state_cb_deregister(SMSM_APPS_STATE,
				SMSM_INIT,
				smsm_state_cb, (void *)0x3456);
		UT_EQ_INT(ret, 2);
		ret = smsm_state_cb_deregister(SMSM_APPS_STATE,
				SMSM_SMDINIT,
				smsm_state_cb, (void *)0x1234);
		UT_EQ_INT(ret, 2);

		i += scnprintf(buf + i, max - i, "Test %d - PASS\n", test_num);
	} while (0);

	return i;
}

=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
static int debug_read_mem(char *buf, int max)
{
	unsigned n;
	struct smem_shared *shared = (void *) MSM_SHARED_RAM_BASE;
	struct smem_heap_entry *toc = shared->heap_toc;
	int i = 0;

	i += scnprintf(buf + i, max - i,
		       "heap: init=%d free=%d remain=%d\n",
		       shared->heap_info.initialized,
		       shared->heap_info.free_offset,
		       shared->heap_info.heap_remaining);

	for (n = 0; n < SMEM_NUM_ITEMS; n++) {
		if (toc[n].allocated == 0)
			continue;
		i += scnprintf(buf + i, max - i,
			       "%04d: offset %08x size %08x\n",
			       n, toc[n].offset, toc[n].size);
	}
	return i;
}

<<<<<<< HEAD
#if (!defined(CONFIG_MSM_SMD_PKG4) && !defined(CONFIG_MSM_SMD_PKG3))
static int debug_read_ch(char *buf, int max)
{
	void *shared;
	int n, i = 0;
	struct smd_alloc_elm *ch_tbl;
	unsigned ch_type;
	unsigned shared_size;

	ch_tbl = smem_find(ID_CH_ALLOC_TBL, sizeof(*ch_tbl) * 64);
	if (!ch_tbl)
		goto fail;

	for (n = 0; n < SMD_CHANNELS; n++) {
		ch_type = SMD_CHANNEL_TYPE(ch_tbl[n].type);
		if (is_word_access_ch(ch_type))
			shared_size =
				sizeof(struct smd_half_channel_word_access);
		else
			shared_size = sizeof(struct smd_half_channel);
		shared = smem_find(ID_SMD_CHANNELS + n,
				2 * shared_size + SMD_BUF_SIZE);

		if (shared == 0)
			continue;
		i += dump_ch(buf + i, max - i, n, shared,
			     (shared + shared_size +
			     SMD_BUF_SIZE), get_half_ch_funcs(ch_type),
			     SMD_BUF_SIZE);
	}

fail:
	return i;
}
#else
static int debug_read_ch(char *buf, int max)
{
	void *shared, *buffer;
	unsigned buffer_sz;
	int n, i = 0;
	struct smd_alloc_elm *ch_tbl;
	unsigned ch_type;
	unsigned shared_size;

	ch_tbl = smem_find(ID_CH_ALLOC_TBL, sizeof(*ch_tbl) * 64);
	if (!ch_tbl)
		goto fail;

	for (n = 0; n < SMD_CHANNELS; n++) {
		ch_type = SMD_CHANNEL_TYPE(ch_tbl[n].type);
		if (is_word_access_ch(ch_type))
			shared_size =
				sizeof(struct smd_half_channel_word_access);
		else
			shared_size = sizeof(struct smd_half_channel);

		shared = smem_find(ID_SMD_CHANNELS + n, 2 * shared_size);

		if (shared == 0)
			continue;

		buffer = smem_get_entry(SMEM_SMD_FIFO_BASE_ID + n, &buffer_sz);

		if (buffer == 0)
			continue;

		i += dump_ch(buf + i, max - i, n, shared,
			     (shared + shared_size),
			     get_half_ch_funcs(ch_type),
			     buffer_sz / 2);
	}

fail:
	return i;
}
#endif

static int debug_read_smem_version(char *buf, int max)
{
	struct smem_shared *shared = (void *) MSM_SHARED_RAM_BASE;
	uint32_t n, version, i = 0;

	for (n = 0; n < 32; n++) {
		version = shared->version[n];
		i += scnprintf(buf + i, max - i,
			       "entry %d: smem = %d  proc_comm = %d\n", n,
			       version >> 16,
			       version & 0xffff);
	}

	return i;
}

/* NNV: revist, it may not be smd version */
static int debug_read_smd_version(char *buf, int max)
{
	uint32_t *smd_ver;
	uint32_t n, version, i = 0;

	smd_ver = smem_alloc(SMEM_VERSION_SMD, 32 * sizeof(uint32_t));

	if (smd_ver)
		for (n = 0; n < 32; n++) {
			version = smd_ver[n];
			i += scnprintf(buf + i, max - i,
				       "entry %d: %d.%d\n", n,
				       version >> 16,
				       version & 0xffff);
		}

	return i;
=======
static int debug_read_ch(char *buf, int max)
{
	struct smd_channel *ch;
	unsigned long flags;
	int i = 0;

	spin_lock_irqsave(&smd_lock, flags);
	list_for_each_entry(ch, &smd_ch_list_dsp, ch_list)
		i += dump_ch(buf + i, max - i, ch);
	list_for_each_entry(ch, &smd_ch_list_modem, ch_list)
		i += dump_ch(buf + i, max - i, ch);
	list_for_each_entry(ch, &smd_ch_closed_list, ch_list)
		i += dump_ch(buf + i, max - i, ch);
	spin_unlock_irqrestore(&smd_lock, flags);

	return i;
}

static int debug_read_version(char *buf, int max)
{
	struct smem_shared *shared = (void *) MSM_SHARED_RAM_BASE;
	unsigned version = shared->version[VERSION_MODEM];
	return sprintf(buf, "%d.%d\n", version >> 16, version & 0xffff);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
}

static int debug_read_build_id(char *buf, int max)
{
	unsigned size;
	void *data;

<<<<<<< HEAD
	data = smem_get_entry(SMEM_HW_SW_BUILD_ID, &size);
=======
	data = smem_item(SMEM_HW_SW_BUILD_ID, &size);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	if (!data)
		return 0;

	if (size >= max)
		size = max;
	memcpy(buf, data, size);

	return size;
}

static int debug_read_alloc_tbl(char *buf, int max)
{
	struct smd_alloc_elm *shared;
	int n, i = 0;

<<<<<<< HEAD
	shared = smem_find(ID_CH_ALLOC_TBL, sizeof(struct smd_alloc_elm[64]));

	if (!shared)
		return 0;

	for (n = 0; n < 64; n++) {
		i += scnprintf(buf + i, max - i,
				"name=%s cid=%d ch type=%d "
				"xfer type=%d ref_count=%d\n",
				shared[n].name,
				shared[n].cid,
				SMD_CHANNEL_TYPE(shared[n].type),
				SMD_XFER_TYPE(shared[n].type),
				shared[n].ref_count);
=======
	shared = smem_find(ID_CH_ALLOC_TBL, sizeof(*shared) * 64);

	for (n = 0; n < 64; n++) {
		if (shared[n].ref_count == 0)
			continue;
		i += scnprintf(buf + i, max - i,
			       "%03d: %-20s cid=%02d type=%03d "
			       "kind=%02d ref_count=%d\n",
			       n, shared[n].name, shared[n].cid,
			       shared[n].ctype & 0xff,
			       (shared[n].ctype >> 8) & 0xf,
			       shared[n].ref_count);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	}

	return i;
}

<<<<<<< HEAD
static int debug_read_intr_mask(char *buf, int max)
{
	uint32_t *smsm;
	int m, n, i = 0;

	smsm = smem_alloc(SMEM_SMSM_CPU_INTR_MASK,
			  SMSM_NUM_ENTRIES * SMSM_NUM_HOSTS * sizeof(uint32_t));

	if (smsm)
		for (m = 0; m < SMSM_NUM_ENTRIES; m++) {
			i += scnprintf(buf + i, max - i, "entry %d:", m);
			for (n = 0; n < SMSM_NUM_HOSTS; n++)
				i += scnprintf(buf + i, max - i,
					       "   host %d: 0x%08x",
					       n, smsm[m * SMSM_NUM_HOSTS + n]);
			i += scnprintf(buf + i, max - i, "\n");
		}

	return i;
}

static int debug_read_intr_mux(char *buf, int max)
{
	uint32_t *smsm;
	int n, i = 0;

	smsm = smem_alloc(SMEM_SMD_SMSM_INTR_MUX,
			  SMSM_NUM_INTR_MUX * sizeof(uint32_t));

	if (smsm)
		for (n = 0; n < SMSM_NUM_INTR_MUX; n++)
			i += scnprintf(buf + i, max - i, "entry %d: %d\n",
				       n, smsm[n]);

	return i;
}

=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
#define DEBUG_BUFMAX 4096
static char debug_buffer[DEBUG_BUFMAX];

static ssize_t debug_read(struct file *file, char __user *buf,
			  size_t count, loff_t *ppos)
{
	int (*fill)(char *buf, int max) = file->private_data;
<<<<<<< HEAD
	int bsize;

	if (*ppos != 0)
		return 0;

	bsize = fill(debug_buffer, DEBUG_BUFMAX);
=======
	int bsize = fill(debug_buffer, DEBUG_BUFMAX);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	return simple_read_from_buffer(buf, count, ppos, debug_buffer, bsize);
}

static const struct file_operations debug_ops = {
	.read = debug_read,
	.open = simple_open,
<<<<<<< HEAD
=======
	.llseek = default_llseek,
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
};

static void debug_create(const char *name, umode_t mode,
			 struct dentry *dent,
			 int (*fill)(char *buf, int max))
{
	debugfs_create_file(name, mode, dent, fill, &debug_ops);
}

<<<<<<< HEAD
static int __init smd_debugfs_init(void)
=======
static int smd_debugfs_init(void)
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
{
	struct dentry *dent;

	dent = debugfs_create_dir("smd", 0);
	if (IS_ERR(dent))
<<<<<<< HEAD
		return PTR_ERR(dent);

	debug_create("ch", 0444, dent, debug_read_ch);
	debug_create("diag", 0444, dent, debug_read_diag_msg);
	debug_create("mem", 0444, dent, debug_read_mem);
	debug_create("version", 0444, dent, debug_read_smd_version);
	debug_create("tbl", 0444, dent, debug_read_alloc_tbl);
	debug_create("modem_err", 0444, dent, debug_modem_err);
	debug_create("modem_err_f3", 0444, dent, debug_modem_err_f3);
	debug_create("print_diag", 0444, dent, debug_diag);
	debug_create("print_f3", 0444, dent, debug_f3);
	debug_create("int_stats", 0444, dent, debug_int_stats);
	debug_create("int_stats_reset", 0444, dent, debug_int_stats_reset);

	/* NNV: this is google only stuff */
=======
		return 1;

	debug_create("ch", 0444, dent, debug_read_ch);
	debug_create("stat", 0444, dent, debug_read_stat);
	debug_create("mem", 0444, dent, debug_read_mem);
	debug_create("version", 0444, dent, debug_read_version);
	debug_create("tbl", 0444, dent, debug_read_alloc_tbl);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	debug_create("build", 0444, dent, debug_read_build_id);

	return 0;
}

<<<<<<< HEAD
static int __init smsm_debugfs_init(void)
{
	struct dentry *dent;

	dent = debugfs_create_dir("smsm", 0);
	if (IS_ERR(dent))
		return PTR_ERR(dent);

	debug_create("state", 0444, dent, debug_read_smsm_state);
	debug_create("intr_mask", 0444, dent, debug_read_intr_mask);
	debug_create("intr_mux", 0444, dent, debug_read_intr_mux);
	debug_create("version", 0444, dent, debug_read_smem_version);
	debug_create("smsm_test", 0444, dent, debug_test_smsm);

	init_completion(&smsm_cb_completion);

	return 0;
}

late_initcall(smd_debugfs_init);
late_initcall(smsm_debugfs_init);
=======
late_initcall(smd_debugfs_init);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
#endif


#define MAX_NUM_SLEEP_CLIENTS		64
#define MAX_SLEEP_NAME_LEN		8

#define NUM_GPIO_INT_REGISTERS		6
#define GPIO_SMEM_NUM_GROUPS		2
#define GPIO_SMEM_MAX_PC_INTERRUPTS	8

struct tramp_gpio_save {
	unsigned int enable;
	unsigned int detect;
	unsigned int polarity;
};

struct tramp_gpio_smem {
	uint16_t num_fired[GPIO_SMEM_NUM_GROUPS];
	uint16_t fired[GPIO_SMEM_NUM_GROUPS][GPIO_SMEM_MAX_PC_INTERRUPTS];
	uint32_t enabled[NUM_GPIO_INT_REGISTERS];
	uint32_t detection[NUM_GPIO_INT_REGISTERS];
	uint32_t polarity[NUM_GPIO_INT_REGISTERS];
};

<<<<<<< HEAD
/*
 * Print debug information on shared memory sleep variables
 */
void smsm_print_sleep_info(uint32_t sleep_delay, uint32_t sleep_limit,
	uint32_t irq_mask, uint32_t wakeup_reason, uint32_t pending_irqs)
{
	unsigned long flags;
	uint32_t *ptr;
	struct tramp_gpio_smem *gpio;

	spin_lock_irqsave(&smem_lock, flags);

	pr_info("SMEM_SMSM_SLEEP_DELAY: %x\n", sleep_delay);
	pr_info("SMEM_SMSM_LIMIT_SLEEP: %x\n", sleep_limit);
=======

void smsm_print_sleep_info(void)
{
	unsigned long flags;
	uint32_t *ptr;
#ifndef CONFIG_ARCH_MSM_SCORPION
	struct tramp_gpio_smem *gpio;
	struct smsm_interrupt_info *int_info;
#endif


	spin_lock_irqsave(&smem_lock, flags);

	ptr = smem_alloc(SMEM_SMSM_SLEEP_DELAY, sizeof(*ptr));
	if (ptr)
		pr_info("SMEM_SMSM_SLEEP_DELAY: %x\n", *ptr);

	ptr = smem_alloc(SMEM_SMSM_LIMIT_SLEEP, sizeof(*ptr));
	if (ptr)
		pr_info("SMEM_SMSM_LIMIT_SLEEP: %x\n", *ptr);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4

	ptr = smem_alloc(SMEM_SLEEP_POWER_COLLAPSE_DISABLED, sizeof(*ptr));
	if (ptr)
		pr_info("SMEM_SLEEP_POWER_COLLAPSE_DISABLED: %x\n", *ptr);
<<<<<<< HEAD
	else
		pr_info("SMEM_SLEEP_POWER_COLLAPSE_DISABLED: missing\n");

	pr_info("SMEM_SMSM_INT_INFO %x %x %x\n",
		irq_mask, pending_irqs, wakeup_reason);
=======

#ifndef CONFIG_ARCH_MSM_SCORPION
	int_info = smem_alloc(SMEM_SMSM_INT_INFO, sizeof(*int_info));
	if (int_info)
		pr_info("SMEM_SMSM_INT_INFO %x %x %x\n",
			int_info->interrupt_mask,
			int_info->pending_interrupts,
			int_info->wakeup_reason);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4

	gpio = smem_alloc(SMEM_GPIO_INT, sizeof(*gpio));
	if (gpio) {
		int i;
		for (i = 0; i < NUM_GPIO_INT_REGISTERS; i++)
			pr_info("SMEM_GPIO_INT: %d: e %x d %x p %x\n",
				i, gpio->enabled[i], gpio->detection[i],
				gpio->polarity[i]);

		for (i = 0; i < GPIO_SMEM_NUM_GROUPS; i++)
			pr_info("SMEM_GPIO_INT: %d: f %d: %d %d...\n",
				i, gpio->num_fired[i], gpio->fired[i][0],
				gpio->fired[i][1]);
<<<<<<< HEAD
	} else
		pr_info("SMEM_GPIO_INT: missing\n");

	spin_unlock_irqrestore(&smem_lock, flags);
}
=======
	}
#else
#endif
	spin_unlock_irqrestore(&smem_lock, flags);
}

>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
