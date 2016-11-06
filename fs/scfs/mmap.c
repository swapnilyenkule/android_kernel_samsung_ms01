/*
 * fs/scfs/mmap.c
 *
 * Copyright (C) 2014 Samsung Electronics Co., Ltd.
<<<<<<< HEAD
 *   Authors: Jongmin Kim <jm45.kim@samsung.com>
=======
 *   Authors: Sunghwan Yun <sunghwan.yun@samsung.com>
 *            Jongmin Kim <jm45.kim@samsung.com>
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 *            Sangwoo Lee <sangwoo2.lee@samsung.com>
 *            Inbae Lee   <inbae.lee@samsung.com>
 *
 * This program has been developed as a stackable file system based on
 * the WrapFS, which was written by:
 *
 * Copyright (C) 1997-2003 Erez Zadok
 * Copyright (C) 2001-2003 Stony Brook University
 * Copyright (C) 2004-2006 International Business Machines Corp.
 *   Author(s): Michael A. Halcrow <mahalcro@us.ibm.com>
 *              Michael C. Thompson <mcthomps@us.ibm.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "scfs.h"
<<<<<<< HEAD

#if MAX_BUFFER_CACHE
/* cluster cache list, and related stuff */
struct read_buffer_cache buffer_cache[MAX_BUFFER_CACHE];
spinlock_t buffer_cache_lock;
int read_buffer_index = 0;
#endif

#ifdef SCFS_ASYNC_READ_PAGES
u64 scfs_readpage_total_count = ATOMIC_INIT(0);
u64 scfs_readpage_io_count = ATOMIC_INIT(0);
u64 scfs_lowerpage_total_count = ATOMIC_INIT(0);
u64 scfs_lowerpage_reclaim_count = ATOMIC_INIT(0);
u64 scfs_op_mode = ATOMIC_INIT(0);
u64 scfs_sequential_page_number = SCFS_SEQUENTIAL_PAGE_NUM;
=======
#include <linux/lzo.h>

#ifdef SCFS_ASYNC_READ_PAGES
#include <linux/freezer.h>
#include <linux/kthread.h>
#include <linux/gfp.h>
#include <linux/sched.h>
#endif


#ifdef SCFS_MULTI_THREAD_COMPRESSION
extern struct kmem_cache *scfs_cbm_cache;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#endif

/**
 * scfs_readpage
 *
 * Parameters:
 * @file: upper file
 * @page: upper page from SCFS inode mapping, data will be copied in here
 *
 * Return:
 * SCFS_SUCCESS if success, otherwise if error
 *
 * Description:
 * - Read in a page by reading a cluster from the file's lower file.
<<<<<<< HEAD
 *   (Reading in a lower cluster for just a single upper page is inevitable
 *    for decompression. The adverse effect of seemingly amplified reads is
 *    mitigated by adding rest of the pages in the cluster to the page cache,
 *    thereby "reading ahead" in its own way.
 * - A pool of cluster buffers are cached for later re-use.
 *   (Repeated reads for the same page will only search through the said list of
 *    buffers without incurring cluster reads.)
 * - Reading & decompressing clusters happen synchronously within the app/user context.
 */

static int scfs_readpage(struct file *file, struct page *page)
=======
 *   (Reading in a cluster for just a single page read is inevitable, but this
 *    "amplified read" and decompressing overhead should be amortized when
 *    other pages in that same cluster is accessed later, and only incurs
 *    memcpy from the cached cluster buffer.)
 * - Recently accessed clusters ("buffer_cache") are cached for later reads.
 */
static inline int _scfs_readpage(struct file *file, struct page *page, int pref_index)
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
{
	struct scfs_inode_info *sii = SCFS_I(page->mapping->host);
	struct scfs_sb_info *sbi = SCFS_S(page->mapping->host->i_sb);
	struct scfs_cluster_buffer buffer = {NULL, NULL, NULL, NULL, 0};
<<<<<<< HEAD
	int ret = SCFS_SUCCESS, compressed = 0;
=======
	int ret = 0, compressed = 0;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	int alloc_membuffer = 1;
	int allocated_index = -1;
	int i;
	char *virt;

	SCFS_PRINT("f:%s i:%d c:0x%x u:0x%x\n",
		file->f_path.dentry->d_name.name,
		page->index, buffer.c_buffer, buffer.u_buffer);
	ASSERT(sii->cluster_size <= SCFS_CLUSTER_SIZE_MAX);

<<<<<<< HEAD
#if (defined(SCFS_READ_PAGES_PROFILE) && defined(SCFS_ASYNC_READ_PAGES))
	scfs_readpage_total_count++;
#endif

#if MAX_BUFFER_CACHE
	/* search cluster for page in buffer_cache */
	spin_lock(&buffer_cache_lock);
	for (i = 0; i < MAX_BUFFER_CACHE; i++) {
		if (buffer_cache[i].inode_number == sii->vfs_inode.i_ino &&
				buffer_cache[i].cluster_number ==
				PAGE_TO_CLUSTER_INDEX(page, sii) &&
				atomic_read(&buffer_cache[i].is_used) != 1) {
			virt = kmap_atomic(page);

			if (buffer_cache[i].is_compressed)
				memcpy(virt, page_address(buffer_cache[i].u_page) +
					PGOFF_IN_CLUSTER(page, sii) * PAGE_SIZE, PAGE_SIZE);
			else
				memcpy(virt, page_address(buffer_cache[i].c_page) +
					PGOFF_IN_CLUSTER(page, sii) * PAGE_SIZE, PAGE_SIZE);

			spin_unlock(&buffer_cache_lock);
			kunmap_atomic(virt);
		//	flush_dcache_page(page);
			SetPageUptodate(page);
			unlock_page(page);

			return SCFS_SUCCESS;
		}
		if (i == read_buffer_index &&
				atomic_read(&buffer_cache[i].is_used) == 1) {
			read_buffer_index++;
			if (read_buffer_index >= MAX_BUFFER_CACHE)
				read_buffer_index = 0;
		}
	}

	/* read a cluster into one of the buffers, and decompress if needed */
	if (atomic_read(&buffer_cache[read_buffer_index].is_used) != 1) {
		buffer.c_page = buffer_cache[read_buffer_index].c_page;
		buffer.u_page = buffer_cache[read_buffer_index].u_page;
		atomic_set(&buffer_cache[read_buffer_index].is_used, 1);
		buffer_cache[read_buffer_index].inode_number = sii->vfs_inode.i_ino;
		buffer_cache[read_buffer_index].cluster_number =
			PAGE_TO_CLUSTER_INDEX(page, sii);
		allocated_index = read_buffer_index++;

		if (read_buffer_index >= MAX_BUFFER_CACHE)
			read_buffer_index = 0;
		alloc_membuffer = 0;
	} else {
		for (i = 0; i < MAX_BUFFER_CACHE; i++) {
			if (atomic_read(&buffer_cache[i].is_used) != 1) {
				buffer.c_page = buffer_cache[i].c_page;
				buffer.u_page = buffer_cache[i].u_page;
				atomic_set(&buffer_cache[i].is_used, 1);
				buffer_cache[i].inode_number = sii->vfs_inode.i_ino;
				buffer_cache[i].cluster_number =
					PAGE_TO_CLUSTER_INDEX(page, sii);
				allocated_index = i;
				read_buffer_index = i + 1;

				if (read_buffer_index >= MAX_BUFFER_CACHE)
					read_buffer_index = 0;
				alloc_membuffer = 0;
				break;
			}
		}
	}
	spin_unlock(&buffer_cache_lock);
#endif

#if (defined(SCFS_READ_PAGES_PROFILE) && defined(SCFS_ASYNC_READ_PAGES))
	scfs_readpage_io_count++;
#endif

	/* buffer_cache is full - alloc from mempool buffer */
=======
#ifdef SCFS_ASYNC_READ_PROFILE
	sbi->scfs_readpage_total_count++;
#endif

#if MAX_BUFFER_CACHE
	/* search buffer_cache first in case the cluster is left cached */
	if (pref_index >= 0 &&
		sbi->buffer_cache[pref_index].ino == sii->vfs_inode.i_ino &&
		sbi->buffer_cache[pref_index].clust_num ==
			PAGE_TO_CLUSTER_INDEX(page, sii) &&
		atomic_read(&sbi->buffer_cache[pref_index].is_used) != 1) {
		spin_lock(&sbi->buffer_cache_lock);
		/* this pref_index is used for another page */
		if (sbi->buffer_cache[pref_index].ino != sii->vfs_inode.i_ino ||
				sbi->buffer_cache[pref_index].clust_num !=
				PAGE_TO_CLUSTER_INDEX(page, sii) ||
				atomic_read(&sbi->buffer_cache[pref_index].is_used) == 1) {
			spin_unlock(&sbi->buffer_cache_lock);
			sbi->buffer_cache_reclaimed_before_used_count++;
			goto pick_slot;
		}
		atomic_set(&sbi->buffer_cache[pref_index].is_used, 1);
		spin_unlock(&sbi->buffer_cache_lock);
		virt = kmap_atomic(page);

		if (sbi->buffer_cache[pref_index].is_compressed)
			memcpy(virt, page_address(sbi->buffer_cache[pref_index].u_page) +
				PGOFF_IN_CLUSTER(page, sii) * PAGE_SIZE, PAGE_SIZE);
		else
			memcpy(virt, page_address(sbi->buffer_cache[pref_index].c_page) +
				PGOFF_IN_CLUSTER(page, sii) * PAGE_SIZE, PAGE_SIZE);

		atomic_set(&sbi->buffer_cache[pref_index].is_used, 0);
		kunmap_atomic(virt);
		SetPageUptodate(page);
		unlock_page(page);
		SCFS_PRINT("%s<h> %d\n",file->f_path.dentry->d_name.name, page->index);

		return pref_index + 1;
	} else if (pref_index >= 0) {
		sbi->buffer_cache_reclaimed_before_used_count++;
		goto pick_slot;
	}

	/* search buffer_cache first in case the cluster is left cached */
	for (i = 0; i < MAX_BUFFER_CACHE; i++) {
		if (sbi->buffer_cache[i].ino == sii->vfs_inode.i_ino &&
			sbi->buffer_cache[i].clust_num ==
				PAGE_TO_CLUSTER_INDEX(page, sii) &&
			atomic_read(&sbi->buffer_cache[i].is_used) != 1) {
			spin_lock(&sbi->buffer_cache_lock);
			if (sbi->buffer_cache[i].ino == sii->vfs_inode.i_ino &&
					sbi->buffer_cache[i].clust_num ==
					PAGE_TO_CLUSTER_INDEX(page, sii) &&
					atomic_read(&sbi->buffer_cache[i].is_used) == 1) {
				spin_unlock(&sbi->buffer_cache_lock);
				goto pick_slot;
			}
			atomic_set(&sbi->buffer_cache[i].is_used, 1);
			spin_unlock(&sbi->buffer_cache_lock);
			virt = kmap_atomic(page);

			if (sbi->buffer_cache[i].is_compressed)
				memcpy(virt, page_address(sbi->buffer_cache[i].u_page) +
					PGOFF_IN_CLUSTER(page, sii) * PAGE_SIZE, PAGE_SIZE);
			else
				memcpy(virt, page_address(sbi->buffer_cache[i].c_page) +
					PGOFF_IN_CLUSTER(page, sii) * PAGE_SIZE, PAGE_SIZE);

			atomic_set(&sbi->buffer_cache[i].is_used, 0);
			kunmap_atomic(virt);
			SetPageUptodate(page);
			unlock_page(page);
			SCFS_PRINT("%s<h> %d\n",
				file->f_path.dentry->d_name.name, page->index);

			return i + 1;
		}
	}

pick_slot:
	/* pick a slot in buffer_cache to use */
	if (atomic_read(&sbi->buffer_cache[sbi->read_buffer_index].is_used) != 1) {
		spin_lock(&sbi->buffer_cache_lock);
		/* this index is used for another page */
		if (atomic_read(&sbi->buffer_cache[sbi->read_buffer_index].is_used) == 1) {
			spin_unlock(&sbi->buffer_cache_lock);
			goto pick_slot_full;
		}
		atomic_set(&sbi->buffer_cache[sbi->read_buffer_index].is_used, 1);
		allocated_index = sbi->read_buffer_index++;
		
		if (sbi->read_buffer_index >= MAX_BUFFER_CACHE)
			sbi->read_buffer_index = 0;

		spin_unlock(&sbi->buffer_cache_lock);
		buffer.c_page = sbi->buffer_cache[allocated_index].c_page;
		buffer.u_page = sbi->buffer_cache[allocated_index].u_page;
		sbi->buffer_cache[allocated_index].ino = sii->vfs_inode.i_ino;
		sbi->buffer_cache[allocated_index].clust_num =
			PAGE_TO_CLUSTER_INDEX(page, sii);
		alloc_membuffer = 0;

		goto real_io;
	} 

pick_slot_full:
	for (i = 0; i < MAX_BUFFER_CACHE; i++) {
		if (atomic_read(&sbi->buffer_cache[i].is_used) != 1) {
			spin_lock(&sbi->buffer_cache_lock);
			/* this index is used for another page */
			if (atomic_read(&sbi->buffer_cache[i].is_used) == 1) {
				spin_unlock(&sbi->buffer_cache_lock);
				continue;
			}

			atomic_set(&sbi->buffer_cache[i].is_used, 1);
			sbi->read_buffer_index = i + 1;

			if (sbi->read_buffer_index >= MAX_BUFFER_CACHE)
				sbi->read_buffer_index = 0;

			spin_unlock(&sbi->buffer_cache_lock);
			buffer.c_page = sbi->buffer_cache[i].c_page;
			buffer.u_page = sbi->buffer_cache[i].u_page;
			sbi->buffer_cache[i].ino = sii->vfs_inode.i_ino;
			sbi->buffer_cache[i].clust_num =
				PAGE_TO_CLUSTER_INDEX(page, sii);
			allocated_index = i;
			alloc_membuffer = 0;
			break;
		}
	}
#endif

real_io:
#ifdef SCFS_ASYNC_READ_PROFILE
	sbi->scfs_readpage_io_count++;
#endif
	/* sanity check & prepare buffers for scfs_read_cluster */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	if (alloc_membuffer == 1 && (buffer.c_page || buffer.c_buffer))
		ASSERT(0);

	if (!buffer.c_page)
		buffer.c_page = scfs_alloc_mempool_buffer(sbi);

	if (!buffer.c_page) {
		SCFS_PRINT_ERROR("c_page malloc failed\n");
<<<<<<< HEAD
		ret = SCFS_ERR_OUT_OF_MEMORY;
=======
		ret = -ENOMEM;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		goto out;
	}

	if (!buffer.c_buffer)
		buffer.c_buffer = page_address(buffer.c_page);

	if (!buffer.c_buffer) {
		SCFS_PRINT_ERROR("c_buffer malloc failed\n");
<<<<<<< HEAD
		ret = SCFS_ERR_OUT_OF_MEMORY;
=======
		ret = -ENOMEM;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		goto out;
	}

	if (!buffer.u_page)
		buffer.u_page = scfs_alloc_mempool_buffer(sbi);

	if (!buffer.u_page) {
		SCFS_PRINT_ERROR("u_page malloc failed\n");
<<<<<<< HEAD
		ret = SCFS_ERR_OUT_OF_MEMORY;
=======
		ret = -ENOMEM;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		goto out;
	}

	if (!buffer.u_buffer)
		buffer.u_buffer = page_address(buffer.u_page);

	if (!buffer.u_buffer) {
		SCFS_PRINT_ERROR("u_buffer malloc failed\n");
<<<<<<< HEAD
		ret = SCFS_ERR_OUT_OF_MEMORY;
		goto out;
	}

=======
		ret = -ENOMEM;
		goto out;
	}
	/* read cluster from lower */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	ret = scfs_read_cluster(file, page, buffer.c_buffer, &buffer.u_buffer, &compressed);

	if (ret) {
		if (ret == -ERANGE)
			SCFS_PRINT_ERROR("file %s error on readpage, OOB. ret %x\n",
				file->f_path.dentry->d_name.name, ret);
		else
			SCFS_PRINT_ERROR("read cluster failed, "
				"file %s page->index %u ret %d\n",
				file->f_path.dentry->d_name.name, page->index, ret);
		goto out;
	}

#if MAX_BUFFER_CACHE
	/* don't need to spinlock, we have is_used=1 for this buffer */
	if (alloc_membuffer != 1)
<<<<<<< HEAD
		buffer_cache[allocated_index].is_compressed = compressed;
#endif

	/* fill page cache with the decompressed or original page */
	virt = kmap_atomic(page);
	if (compressed)
		memcpy(virt, page_address(buffer.u_page) + PGOFF_IN_CLUSTER(page, sii) * PAGE_SIZE,
			PAGE_SIZE);
	else
		memcpy(virt, page_address(buffer.c_page) + PGOFF_IN_CLUSTER(page, sii) * PAGE_SIZE,
			PAGE_SIZE);

	kunmap_atomic(virt);
//	flush_dcache_page(page);
	SetPageUptodate(page);

#if MAX_BUFFER_CACHE
	if (alloc_membuffer != 1) {
		spin_lock(&buffer_cache_lock);
		atomic_set(&buffer_cache[allocated_index].is_used, 0);
		spin_unlock(&buffer_cache_lock);
	}
#endif
=======
		sbi->buffer_cache[allocated_index].is_compressed = compressed;
#endif

#ifdef SCFS_REMOVE_NO_COMPRESSED_UPPER_MEMCPY
	/* fill page cache with the decompressed or original page */
	if (compressed) {
		virt = kmap_atomic(page);
		memcpy(virt, page_address(buffer.u_page) +
			PGOFF_IN_CLUSTER(page, sii) * PAGE_SIZE, PAGE_SIZE);
		kunmap_atomic(virt);
	}
#else
	/* fill page cache with the decompressed/original data */
	virt = kmap_atomic(page);
	if (compressed)
		memcpy(virt, page_address(buffer.u_page) +
			PGOFF_IN_CLUSTER(page, sii) * PAGE_SIZE, PAGE_SIZE);
	else
		memcpy(virt, page_address(buffer.c_page) +
			PGOFF_IN_CLUSTER(page, sii) * PAGE_SIZE, PAGE_SIZE);
	kunmap_atomic(virt);
#endif
	SetPageUptodate(page);

#if MAX_BUFFER_CACHE
#ifndef SCFS_REMOVE_NO_COMPRESSED_UPPER_MEMCPY
	if (alloc_membuffer != 1) {
		atomic_set(&sbi->buffer_cache[allocated_index].is_used, 0);
	}
#else
	if (alloc_membuffer != 1 && compressed) {
		atomic_set(&sbi->buffer_cache[allocated_index].is_used, 0);
	} else if (alloc_membuffer != 1) {
		spin_lock(&sbi->buffer_cache_lock);
		sbi->buffer_cache[allocated_index].ino = -1;
		sbi->buffer_cache[allocated_index].clust_num = -1;
		sbi->buffer_cache[allocated_index].is_compressed = -1;
		atomic_set(&sbi->buffer_cache[allocated_index].is_used, -1);
		spin_unlock(&sbi->buffer_cache_lock);
	}
#endif
#endif
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

out:
	unlock_page(page);

	if (alloc_membuffer == 1) {
<<<<<<< HEAD
=======
		sbi->buffer_cache_overflow_count_smb++;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		scfs_free_mempool_buffer(buffer.c_page, sbi);
		scfs_free_mempool_buffer(buffer.u_page, sbi);
	}

	SCFS_PRINT("-f:%s i:%d c:0x%x u:0x%x\n",
		file->f_path.dentry->d_name.name,
		page->index, buffer.c_buffer, buffer.u_buffer);

<<<<<<< HEAD
	return ret;
}

#ifdef SCFS_ASYNC_READ_PAGES
#include <linux/freezer.h>
#include <linux/kthread.h>
#include <linux/gfp.h>
#include <linux/sched.h>

/* memory and file buffer for scfs_readpages */
struct page *page_buffer_smb[MAX_PAGE_BUFFER_SIZE_SMB];
struct file *file_buffer_smb[MAX_PAGE_BUFFER_SIZE_SMB];

/* when page_buffer_smb and file_buffer_smb is full, then this filling_index is set to MAX_PAGE_BUFFER_SIZE */
int page_buffer_next_filling_index_smb;
int page_buffer_next_io_index_smb;
spinlock_t spinlock_smb;

struct task_struct *smb_task[NR_CPUS] = {NULL, };

int smb_init(void) {
	int i, j;
	//int cpu[NR_CPUS];
	//struct cpumask cpus;

	page_buffer_next_filling_index_smb = 0;
	page_buffer_next_io_index_smb = 0;
	spin_lock_init(&spinlock_smb);

	for (i = 0; i < NR_CPUS; i++) {
		smb_task[i] = kthread_run(smb_thread, NULL, "scfs_mb%d", i);

		if (IS_ERR(smb_task[i])) {
			SCFS_PRINT_ERROR("smb_init: creating kthread failed\n");

			for (j=0; j < i; j++)
				kthread_stop(smb_task[j]);
			return -ENOMEM;
		}
=======
	SCFS_PRINT("%s<r> %d\n",file->f_path.dentry->d_name.name, page->index);

	if (ret < 0)
		return ret;
	else if (alloc_membuffer != 1)
		return allocated_index + 1;
	else
		return 0;
}

static int scfs_readpage(struct file *file, struct page *page)
{
	int ret;
#ifdef SCFS_ASYNC_READ_PROFILE
	struct scfs_sb_info *sbi = SCFS_S(file->f_mapping->host->i_sb);

	atomic_inc(&sbi->scfs_standby_readpage_count);
#endif
	ret = _scfs_readpage(file, page, -1);
#ifdef SCFS_ASYNC_READ_PROFILE
	atomic_dec(&sbi->scfs_standby_readpage_count);
#endif
	return (ret >= 0 ? 0 : ret);
}

#ifdef SCFS_ASYNC_READ_PAGES
int smb_init(struct scfs_sb_info *sbi)
{
	int i, j;

	for (i = 0; i < NR_CPUS; i++) {
		sbi->smb_task[i] = kthread_run(smb_thread, sbi, "scfs_mb%d", i);

		if (IS_ERR(sbi->smb_task[i])) {
			SCFS_PRINT_ERROR("smb_init: creating kthread failed\n");

			for (j = 0; j < i; j++)
				kthread_stop(sbi->smb_task[j]);
			return -ENOMEM;
		}

#ifdef SCFS_SMB_THREAD_CPU_AFFINITY
		{
			struct cpumask cpus[NR_CPUS];
			cpumask_clear(&cpus[i]);
			cpumask_set_cpu(i, &cpus[i]);

			if (sched_setaffinity(sbi->smb_task[i]->pid, &cpus[i])) {
				SCFS_PRINT_ERROR("smb_init: set CPU affinity failed\n");
			}
		}
#endif
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	}

	return 0;
}

<<<<<<< HEAD
void wakeup_smb_thread(void) {
	int length = 0, io_index, filling_index;

	io_index = page_buffer_next_io_index_smb;
	filling_index = page_buffer_next_filling_index_smb;
=======
void smb_destroy(struct scfs_sb_info *sbi)
{
	int i;

	for(i = 0 ; i < NR_CPUS ; i++) {
		if(sbi->smb_task[i])
			kthread_stop(sbi->smb_task[i]);
		sbi->smb_task[i] = NULL;
	}
}


/* scaling # of threads will be woken up, on-demand */
void wakeup_smb_thread(struct scfs_sb_info *sbi)
{
	u32 length = 0, io_index, filling_index;

	spin_lock(&sbi->spinlock_smb);
	io_index = sbi->page_buffer_next_io_index_smb;
	filling_index = sbi->page_buffer_next_filling_index_smb;
	spin_unlock(&sbi->spinlock_smb);

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	if (filling_index == MAX_PAGE_BUFFER_SIZE_SMB)
		length = MAX_PAGE_BUFFER_SIZE_SMB;
	else if (filling_index > io_index)
		length = filling_index - io_index;
	else if (filling_index < io_index)
		length = (MAX_PAGE_BUFFER_SIZE_SMB - io_index) + filling_index;
	else if (filling_index == io_index) 
		length = 0;

<<<<<<< HEAD
	if (length > 0 && smb_task[1]) wake_up_process(smb_task[1]);
	if (length >= SMB_THREAD_THRESHOLD_4) {
		if (smb_task[2]) wake_up_process(smb_task[2]);
		if (smb_task[3]) wake_up_process(smb_task[3]);
		if (smb_task[0]) wake_up_process(smb_task[0]);
	} else if (length >= SMB_THREAD_THRESHOLD_3) {
		if (smb_task[2]) wake_up_process(smb_task[2]);
		if (smb_task[3]) wake_up_process(smb_task[3]);
	} else if (length >= SMB_THREAD_THRESHOLD_2) {
		if (smb_task[2]) wake_up_process(smb_task[2]);
	}
}

int smb_thread(void *nothing)
{
	int length = 0, io_index, filling_index;
=======
	if (length > 0 && sbi->smb_task[0] && !sbi->smb_task_status[0])
		wake_up_process(sbi->smb_task[0]);
#if (NR_CPUS > 1)
	if (length >= SMB_THREAD_THRESHOLD_2 &&sbi->smb_task[1] && !sbi->smb_task_status[1])
		wake_up_process(sbi->smb_task[1]);
#if (NR_CPUS > 2)
	if (length >= SMB_THREAD_THRESHOLD_3 && sbi->smb_task[2] && !sbi->smb_task_status[2])
		wake_up_process(sbi->smb_task[2]);
#if (NR_CPUS > 3)
	if (length >= SMB_THREAD_THRESHOLD_4 && sbi->smb_task[3] && !sbi->smb_task_status[3])
		wake_up_process(sbi->smb_task[3]);
#endif
#endif
#endif
}

int smb_thread(void *data)
{
	u32 length = 0, io_index, filling_index;
	struct scfs_sb_info *sbi = (struct scfs_sb_info *)data;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	struct page *page;
	struct page *temp_page;
	struct page *page_buffer[3] = {NULL, NULL, NULL};
	struct file *file;
	struct file *temp_file = NULL;
	struct scfs_inode_info *sii;
	int cluster_number = -1;
	int page_buffer_count = 0;
	int i;
<<<<<<< HEAD

	set_freezable();

	while (!kthread_should_stop()) {
		/* If page buffer don't have any page, then I sleep.
		 * When page buffer has any pages, then I will be awaked by read request processes
		 */
		set_current_state(TASK_INTERRUPTIBLE);
		spin_lock(&spinlock_smb);

		/* calculate number of pages of page buffer */
		io_index = page_buffer_next_io_index_smb;
		filling_index = page_buffer_next_filling_index_smb;
		if (filling_index == MAX_PAGE_BUFFER_SIZE_SMB) {
			length = MAX_PAGE_BUFFER_SIZE_SMB;
			page_buffer_next_filling_index_smb = page_buffer_next_io_index_smb;
		}
		else if (filling_index > io_index)
=======
	int prev_cbi = 0;

	set_freezable();

	/* handle any queued-up read requests, or else go back to sleep */
	while (!kthread_should_stop()) {
		set_current_state(TASK_INTERRUPTIBLE);
		spin_lock(&sbi->spinlock_smb);

		/* calculate number of pages of page buffer */
		io_index = sbi->page_buffer_next_io_index_smb;
		filling_index = sbi->page_buffer_next_filling_index_smb;

		if (filling_index == MAX_PAGE_BUFFER_SIZE_SMB) {
			length = MAX_PAGE_BUFFER_SIZE_SMB;
			sbi->page_buffer_next_filling_index_smb =
				sbi->page_buffer_next_io_index_smb;
		} else if (filling_index > io_index)
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
			length = filling_index - io_index;
		else if (filling_index < io_index)
			length = (MAX_PAGE_BUFFER_SIZE_SMB - io_index) + filling_index;
		else if (filling_index == io_index) 
			length = 0;
<<<<<<< HEAD
		page_buffer_count = 0;

		/* If page buffer has some pages, then execute readpage for the first page and
		 * related pages with cluster of first page
		 */
		if (length > 0) {
			__set_current_state(TASK_RUNNING);
			page = page_buffer_smb[page_buffer_next_io_index_smb];
			file = file_buffer_smb[page_buffer_next_io_index_smb];

			page_buffer_next_io_index_smb++;
			if (page_buffer_next_io_index_smb >= MAX_PAGE_BUFFER_SIZE_SMB)
				page_buffer_next_io_index_smb = 0;
=======

		page_buffer_count = 0;

		/* the requested page, as well as subsequent pages in the same cluster,
		 * will be serviced, in two separate readpage calls
		 */
		if (length > 0) {
			__set_current_state(TASK_RUNNING);
			page = sbi->page_buffer_smb[sbi->page_buffer_next_io_index_smb];
			file = sbi->file_buffer_smb[sbi->page_buffer_next_io_index_smb];
			sbi->page_buffer_next_io_index_smb++;

			if (sbi->page_buffer_next_io_index_smb >= MAX_PAGE_BUFFER_SIZE_SMB)
				sbi->page_buffer_next_io_index_smb = 0;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

			length--;
			sii = SCFS_I(page->mapping->host);
			cluster_number = PAGE_TO_CLUSTER_INDEX(page, sii);

			while (length-- > 0) {
<<<<<<< HEAD
				temp_page = page_buffer_smb[page_buffer_next_io_index_smb];
				temp_file = file_buffer_smb[page_buffer_next_io_index_smb];

				if ((temp_file == file) && (cluster_number == PAGE_TO_CLUSTER_INDEX(temp_page, sii))) {
					page_buffer[page_buffer_count++] = temp_page;
			page_buffer_next_io_index_smb++;
			if (page_buffer_next_io_index_smb >= MAX_PAGE_BUFFER_SIZE_SMB)
				page_buffer_next_io_index_smb = 0;
				} else break;
			}
			spin_unlock(&spinlock_smb);

			/* read first page */
			scfs_readpage(file, page);
			atomic_long_dec(&file->f_count);

			/* read related pages with cluster of first page*/
			for (i = 0; i < page_buffer_count; i++) {
				scfs_readpage(file, page_buffer[i]);
				atomic_long_dec(&file->f_count);
			}
			/* this may leave some of the queued pages hanging, so let's be less nice and not sleep */
			//cond_resched();
		}
		else {
			spin_unlock(&spinlock_smb);
			schedule();
=======
				temp_page = sbi->page_buffer_smb[sbi->page_buffer_next_io_index_smb];
				temp_file = sbi->file_buffer_smb[sbi->page_buffer_next_io_index_smb];

				if ((temp_file == file) &&
					(cluster_number == PAGE_TO_CLUSTER_INDEX(temp_page, sii))) {
					page_buffer[page_buffer_count++] = temp_page;
					sbi->page_buffer_next_io_index_smb++;

					if (sbi->page_buffer_next_io_index_smb >=
						MAX_PAGE_BUFFER_SIZE_SMB)
						sbi->page_buffer_next_io_index_smb = 0;
				} else
					break;
			}
			spin_unlock(&sbi->spinlock_smb);

			/* read first page */
			prev_cbi = _scfs_readpage(file, page, -1);
			fput(SCFS_F(file)->lower_file);
			fput(file);
			page_cache_release(page);

			/* read related pages with cluster of first page*/
			for (i = 0; i < page_buffer_count; i++) {
				prev_cbi = _scfs_readpage(file, page_buffer[i], prev_cbi - 1);
				fput(SCFS_F(file)->lower_file);
				fput(file);
				page_cache_release(page_buffer[i]);
			}
		} else {
			//sbi->smb_task_status[xx] = 0;
			spin_unlock(&sbi->spinlock_smb);
			schedule();
			//sbi->smb_task_status[xx] = 1;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		}
	}

	return 0;
}

/**
 * scfs_readpages
 *
 * Parameters:
 * @file: upper file
 * @*mapping: address_space struct for the file
 * @*pages: list of pages to read in
 * @nr_pages: number of pages to read in
 *
 * Return:
 * SCFS_SUCCESS if success, otherwise if error
 *
 * Description:
 * - Asynchronously read pages for readahead. A scaling number of background threads
<<<<<<< HEAD
 *   will read & decompress clusters in a deferred manner.
=======
 *   will read & decompress them in a slightly deferred but parallelized manner.
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 */
static int
scfs_readpages(struct file *file, struct address_space *mapping,
		struct list_head *pages, unsigned nr_pages)
{
	struct scfs_inode_info *sii = SCFS_I(file->f_mapping->host);
<<<<<<< HEAD
	int page_idx, page_idx_readahead = 1024, ret = SCFS_SUCCESS;
	struct page *page;
	struct file *lower_file = NULL;
	loff_t i_size;
	int readahead_page = 0;
=======
	struct scfs_sb_info *sbi = SCFS_S(file->f_mapping->host->i_sb);
	struct file *lower_file = NULL;
	struct page *page;
	struct scfs_cinfo cinfo;
	loff_t i_size;
	pgoff_t start, end;
	int page_idx, page_idx_readahead = 1024, ret = 0;
	int readahead_page = 0;
	int prev_cbi = 0;
	int prev_cluster = -1, cur_cluster = -1;
	int cluster_idx = 0;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

	i_size = i_size_read(&sii->vfs_inode);
	if (!i_size) {
		SCFS_PRINT("file %s: i_size is zero, "
			"flags 0x%x sii->clust_info_size %d\n",
			file->f_path.dentry->d_name.name, sii->flags,
			sii->cinfo_array_size);
<<<<<<< HEAD
		return SCFS_SUCCESS;
	}

/* important: this routine add pages only to lower page cache (No upper page cache).
 * so, instead of using lower readpages we use vfs_read like IS_COMPRESSED */
#if 0
	if (!IS_COMPRESSED(sii)) {
		lower_file = scfs_lower_file(file);
		if (!lower_file) {
			SCFS_PRINT_ERROR("file %s: lower file is null!\n",
			        file->f_path.dentry->d_name.name);
			ASSERT(0);
			return -EINVAL;
		}
		/* important: we should add pages to upper page cache not lower page cache */
		return lower_file->f_mapping->a_ops->readpages(lower_file, lower_file->f_mapping, pages, nr_pages);
	}
=======
		return 0;
	}

#ifdef SCFS_ASYNC_READ_PROFILE
	atomic_add(nr_pages, &sbi->scfs_standby_readpage_count);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#endif

#ifdef SCFS_NOTIFY_RANDOM_READ
	lower_file = scfs_lower_file(file);
	if (!lower_file) {
		SCFS_PRINT_ERROR("file %s: lower file is null!\n",
		        file->f_path.dentry->d_name.name);
		return -EINVAL;
	}

	/* if the read request was random (enough), hint it to the lower file. 
	 * scfs_sequential_page_number is the tunable threshold.
	 * filemap.c will later on refer to this FMODE_RANDOM flag.
	*/
	spin_lock(&lower_file->f_lock);
<<<<<<< HEAD
	if (nr_pages > scfs_sequential_page_number)
=======
	if (nr_pages > sbi->scfs_sequential_page_number)
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		lower_file->f_mode &= ~FMODE_RANDOM;
	else
		lower_file->f_mode |= FMODE_RANDOM;
	spin_unlock(&lower_file->f_lock);
#endif
<<<<<<< HEAD
=======
	lower_file = scfs_lower_file(file);
	page = list_entry(pages->prev, struct page, lru);
	cluster_idx = page->index / (sii->cluster_size / PAGE_SIZE);

	if (sii->compressed) {
		mutex_lock(&sii->cinfo_mutex);
		ret = get_cluster_info(file, cluster_idx, &cinfo);
		mutex_unlock(&sii->cinfo_mutex);
		if (ret) {
			SCFS_PRINT_ERROR("err in get_cluster_info, ret : %d,"
				"i_size %lld\n", ret, i_size);
			return ret;
		}

		if (!cinfo.size || cinfo.size > sii->cluster_size) {
			SCFS_PRINT_ERROR("file %s: cinfo is invalid, "
				"clust %u cinfo.size %u\n",
				file->f_path.dentry->d_name.name,
				cluster_idx, cinfo.size);
			return -EINVAL;
		}
		start = (pgoff_t)(cinfo.offset / PAGE_SIZE);
	} else {
		start = (pgoff_t)(cluster_idx * sii->cluster_size / PAGE_SIZE);
	}

	cluster_idx = (page->index + nr_pages - 1) / (sii->cluster_size / PAGE_SIZE);
	if (sii->compressed) {
		mutex_lock(&sii->cinfo_mutex);
		ret = get_cluster_info(file, cluster_idx, &cinfo);
		mutex_unlock(&sii->cinfo_mutex);
		if (ret) {
			SCFS_PRINT_ERROR("err in get_cluster_info, ret : %d,"
				"i_size %lld\n", ret, i_size);
			return ret;
		}

		if (!cinfo.size || cinfo.size > sii->cluster_size) {
			SCFS_PRINT_ERROR("file %s: cinfo is invalid, "
				"clust %u cinfo.size %u\n",
				file->f_path.dentry->d_name.name,
				cluster_idx, cinfo.size);
			return -EINVAL;
		}
		end = (pgoff_t)((cinfo.offset + cinfo.size -1) / PAGE_SIZE);
	} else {
		end = (pgoff_t)(((cluster_idx + 1) * sii->cluster_size - 1) / PAGE_SIZE);
		/* check upper inode size */

		/* out of range? on compressed file, it is handled returning error,
		   which one is right? */
		if (end > (i_size / PAGE_SIZE))
			end = (i_size / PAGE_SIZE);
	}
	force_page_cache_readahead(lower_file->f_mapping, lower_file,
		start, (unsigned long)(end - start +1));
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

	for (page_idx = 0; page_idx < nr_pages; page_idx++) {
		page = list_entry(pages->prev, struct page, lru);
		list_del(&page->lru);

		if (PageReadahead(page))
			page_idx_readahead = page_idx;

		ret = add_to_page_cache_lru(page, mapping,
				      page->index, GFP_KERNEL);
		if (ret) {
			SCFS_PRINT("adding to page cache failed, "
				"page %x page->idx %d ret %d\n",
				page, page->index, ret);
			page_cache_release(page);
			continue;
		}

<<<<<<< HEAD
		/* memory buffer is full or synchronous read request - call scfs_readpage to read now */
		if (page_buffer_next_filling_index_smb ==
				MAX_PAGE_BUFFER_SIZE_SMB || page_idx < page_idx_readahead) {
			scfs_readpage(file, page);
		} else {
			/* At this point of time, page buffer is not full. So store this page to page buffer */
			spin_lock(&spinlock_smb);

			/* When we don't increase file->f_count here, then kernel panic will occur when smb_thread access file variable */
			atomic_long_inc(&file->f_count);
			page_buffer_smb[page_buffer_next_filling_index_smb] = page;
			file_buffer_smb[page_buffer_next_filling_index_smb++] = file;

			/* check whether page buffer is full and set page buffer full if needed */
			if (((page_buffer_next_filling_index_smb == MAX_PAGE_BUFFER_SIZE_SMB) &&
				page_buffer_next_io_index_smb == 0) ||
				(page_buffer_next_filling_index_smb == page_buffer_next_io_index_smb))
				page_buffer_next_filling_index_smb = MAX_PAGE_BUFFER_SIZE_SMB;
			else if (page_buffer_next_filling_index_smb == MAX_PAGE_BUFFER_SIZE_SMB)
				page_buffer_next_filling_index_smb = 0;
			spin_unlock(&spinlock_smb);
			if (++readahead_page % 4 == 0) wakeup_smb_thread();
		}

		page_cache_release(page);
	}

	if (readahead_page % 4) wakeup_smb_thread();

	return SCFS_SUCCESS;
}
#endif
=======
		/* memory buffer is full or synchronous read request -
		   call scfs_readpage to read now */
		if (sbi->page_buffer_next_filling_index_smb ==
				MAX_PAGE_BUFFER_SIZE_SMB || page_idx < page_idx_readahead) {
			cur_cluster = PAGE_TO_CLUSTER_INDEX(page, sii);

			if (prev_cluster == cur_cluster && prev_cbi > 0)
				prev_cbi = _scfs_readpage(file, page, prev_cbi - 1);
			else
				prev_cbi = _scfs_readpage(file, page, -1);

			prev_cluster = cur_cluster;
			page_cache_release(page); /* refer line 701 */
		} else {
			spin_lock(&sbi->spinlock_smb);

			/* Queue is not full so add the page into the queue.
			   Also, here we increase file->f_count to protect
			   the file structs from multi-threaded accesses */
			atomic_long_inc(&SCFS_F(file)->lower_file->f_count);
			atomic_long_inc(&file->f_count);
			sbi->page_buffer_smb[sbi->page_buffer_next_filling_index_smb] = page;
			sbi->file_buffer_smb[sbi->page_buffer_next_filling_index_smb++] = file;

			/* check whether page buffer is full and set page buffer full if needed */
			if (((sbi->page_buffer_next_filling_index_smb == MAX_PAGE_BUFFER_SIZE_SMB) &&
				sbi->page_buffer_next_io_index_smb == 0) ||
				(sbi->page_buffer_next_filling_index_smb ==
				sbi->page_buffer_next_io_index_smb))
				sbi->page_buffer_next_filling_index_smb = MAX_PAGE_BUFFER_SIZE_SMB;
			else if (sbi->page_buffer_next_filling_index_smb == MAX_PAGE_BUFFER_SIZE_SMB)
				sbi->page_buffer_next_filling_index_smb = 0;
			spin_unlock(&sbi->spinlock_smb);
			++readahead_page;
		}
		//page_cache_release(page);
	}

	if (readahead_page > 0)
		wakeup_smb_thread(sbi);

	SCFS_PRINT("<e>\n");

#ifdef SCFS_ASYNC_READ_PROFILE
	atomic_sub(nr_pages, &sbi->scfs_standby_readpage_count);
#endif
	return 0;
}
#endif /* SCFS_ASYNC_READ_PAGES */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

/**
 * scfs_write_begin
 * @file: The scfs file
<<<<<<< HEAD
 * @mapping: The scfs object
 * @pos: The file offset at which to start writing
 * @len: Length of the write
 * @flags: Various flags
 * @pagep: Pointer to return the page
 * @fsdata: Pointer to return fs data (unused)
 *
 * SCFS doesn't handle Random Write. So, this function will return EINVAL if pos < i_size for current file.
=======
 * @mapping: The file's address_space
 * @pos: The write starting position, in bytes
 * @len: Bytes to write in this page
 * @flags: Various flags
 * @pagep: Pointer to return the page
 * @fsdata: Pointer to return fs-specific data (unused)
 *
 * Description:
 * - Prepare a page write, which may require a cluster read and re-compression
 *   for partially written clusters at the end of a given file. Cluster info list,
 *   as well as the cluster buffer for the cluster to be written, shall be prepped
 *   accordingly.
 * - Currently SCFS doesn't support random writes, so this function will return 
 *   -EINVAL if pos < i_size.
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 *
 */
static int scfs_write_begin(struct file *file, struct address_space *mapping,
			loff_t pos, unsigned len, unsigned flags,
			struct page **pagep, void **fsdata)
{
	pgoff_t index = pos >> PAGE_CACHE_SHIFT;
	struct page *page;
	struct file *lower_file = NULL;
	struct scfs_inode_info *sii = SCFS_I(mapping->host);
	struct scfs_cinfo clust_info;
	int ret = 0;

	SCFS_PRINT("f:%s pos:%lld, len:%d s:%lld\n",
			file->f_path.dentry->d_name.name, pos, len, 
			i_size_read(&sii->vfs_inode));

	page = grab_cache_page_write_begin(mapping, index, flags);
	if (!page)
		return -ENOMEM;
		
	*pagep = page;
	if (pos != i_size_read(&sii->vfs_inode)) {
		SCFS_PRINT("File %s RANDOM write access! pos = %lld, i_size = %lld\n",
			file->f_path.dentry->d_name.name,pos,i_size_read(&sii->vfs_inode));
		ret = -EINVAL;
		goto out;
	}

	lower_file = scfs_lower_file(file);
	if (!lower_file) {
		SCFS_PRINT_ERROR("lower file is null!\n");
		ret = -EIO;
		goto out;
	}	

<<<<<<< HEAD
	if (IS_COMPRESSED(sii)) {
=======
	if (IS_COMPRESSABLE(sii)) {
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		struct cinfo_entry *info_entry;
		ret = scfs_get_comp_buffer(sii);
		if (ret)
			goto out;

<<<<<<< HEAD
		mutex_lock(&sii->cinfo_list_mutex);
		if (list_empty(&sii->cinfo_list)) {
			info_entry = scfs_alloc_cinfo_entry(PAGE_TO_CLUSTER_INDEX(page,sii), sii);
			if (!info_entry) {
				mutex_unlock(&sii->cinfo_list_mutex);
=======
		mutex_lock(&sii->cinfo_mutex);
		if (list_empty(&sii->cinfo_list)) {
			/* first cluster write, probably, since create or open */
			info_entry = scfs_alloc_cinfo_entry(PAGE_TO_CLUSTER_INDEX(page,sii), sii);
			if (!info_entry) {
				mutex_unlock(&sii->cinfo_mutex);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
				SCFS_PRINT_ERROR("Cannot alloc new cluster_info.");
				ret = -ENOMEM;
				goto out;
			}

			if (PAGE_TO_CLUSTER_INDEX(page,sii) == 0)
				info_entry->cinfo.offset = 0;

<<<<<<< HEAD
			if (CLUSTER_EXISTS(sii, PAGE_TO_CLUSTER_INDEX(page,sii))) {
				mutex_unlock(&sii->cinfo_list_mutex);
				ret = get_cluster_info(file, PAGE_TO_CLUSTER_INDEX(page,sii),
						&clust_info);
				mutex_lock(&sii->cinfo_list_mutex);
				if (ret) {
					mutex_unlock(&sii->cinfo_list_mutex);
=======
			/* lower cluster already exists so we must be writing on the last cluster */
			if (IS_CLUSTER_EXIST(sii, PAGE_TO_CLUSTER_INDEX(page,sii))) {
				ret = get_cluster_info(file, PAGE_TO_CLUSTER_INDEX(page,sii),
						&clust_info);
				if (ret) {
					mutex_unlock(&sii->cinfo_mutex);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
					SCFS_PRINT_ERROR("page is in file, " \
						"but cannot get cluster info.");
					goto out;
				}
				info_entry->cinfo.offset = clust_info.offset;
<<<<<<< HEAD
				ret = scfs_get_cluster_from_lower(sii, lower_file, clust_info);
				if (ret) {
					mutex_unlock(&sii->cinfo_list_mutex);
=======
				ret = scfs_get_cluster_from_lower(file, clust_info);
				if (ret) {
					mutex_unlock(&sii->cinfo_mutex);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
					SCFS_PRINT_ERROR("Fail to get lower data.");
					goto out;
				}				

				if (!PageUptodate(page))
					sync_page_from_buffer(page, sii->cluster_buffer.u_buffer);
				
			} else if (PAGE_TO_CLUSTER_INDEX(page,sii) > 0 
<<<<<<< HEAD
				&& CLUSTER_EXISTS(sii, PAGE_TO_CLUSTER_INDEX(page,sii) - 1)) {
				ret = get_cluster_info(file, PAGE_TO_CLUSTER_INDEX(page,sii) - 1,
						&clust_info);
				if (ret) {
					mutex_unlock(&sii->cinfo_list_mutex);
=======
				&& IS_CLUSTER_EXIST(sii, PAGE_TO_CLUSTER_INDEX(page,sii) - 1)) {
				/* we must be adding a new cluster with this page write */
				ret = get_cluster_info(file, PAGE_TO_CLUSTER_INDEX(page,sii) - 1,
						&clust_info);
				if (ret) {
					mutex_unlock(&sii->cinfo_mutex);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
					SCFS_PRINT_ERROR("page is in file, but cannot get cluster info.");
					goto out;
				}

				info_entry->cinfo.offset = clust_info.offset+clust_info.size;
<<<<<<< HEAD
				if (clust_info.size%SCFS_CLUSTER_ALIGN) {
					info_entry->cinfo.offset += 
					(SCFS_CLUSTER_ALIGN - (clust_info.size%SCFS_CLUSTER_ALIGN));
				}
			}
		} else {
=======
				if (clust_info.size%SCFS_CLUSTER_ALIGN_BYTE) {
					info_entry->cinfo.offset += (SCFS_CLUSTER_ALIGN_BYTE -
						(clust_info.size%SCFS_CLUSTER_ALIGN_BYTE));
				}
			}
		} else {
			/* cinfo list is not empty, cluster writes must have happened */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
			struct cinfo_entry *new_list = NULL;

			if (!PageUptodate(page) && pos & (PAGE_CACHE_SIZE - 1)) {
				SCFS_PRINT_ERROR("Current page was reclaimed " \
					"before be written to lower\n");
				ASSERT(0);
			}
			info_entry = list_entry(sii->cinfo_list.prev,
				struct cinfo_entry, entry);
			if (info_entry->current_cluster_idx != PAGE_TO_CLUSTER_INDEX(page,sii)) {
				if (info_entry->current_cluster_idx ==
						PAGE_TO_CLUSTER_INDEX(page,sii) - 1)
					new_list = scfs_alloc_cinfo_entry(PAGE_TO_CLUSTER_INDEX(page,sii),
							sii);
				else
					info_entry = NULL;			
			}

			if (!info_entry) {
<<<<<<< HEAD
				mutex_unlock(&sii->cinfo_list_mutex);
=======
				mutex_unlock(&sii->cinfo_mutex);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
				SCFS_PRINT_ERROR("Cannot alloc new cluster_info.");
				ret = -ENOMEM;
				goto out;
			}

<<<<<<< HEAD
=======
#ifndef SCFS_MULTI_THREAD_COMPRESSION
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
			if (new_list) {
				new_list->cinfo.offset =
					info_entry->cinfo.offset +
					info_entry->cinfo.size;
<<<<<<< HEAD
				if(info_entry->cinfo.size % SCFS_CLUSTER_ALIGN)
					new_list->cinfo.offset += 
					(SCFS_CLUSTER_ALIGN -
					(info_entry->cinfo.size % SCFS_CLUSTER_ALIGN));
			}
		}		
		mutex_unlock(&sii->cinfo_list_mutex);
	} else {
		/* Fake info list : We need to write meta at the EOF. 
		 * But, write meta check whether info list is empty or not.
		 * In this case, we cannot write META because we don't use cluster info list and also list is empty.
		 * So, We add one fake info list to sii->cinfo_list
		*/
		if (list_empty(&sii->cinfo_list)) {
			struct cinfo_entry *info_entry = NULL;
			//struct scfs_sb_info *sbi = SCFS_S(sii->vfs_inode.i_sb);
=======
				if (info_entry->cinfo.size % SCFS_CLUSTER_ALIGN_BYTE)
					new_list->cinfo.offset +=
					(SCFS_CLUSTER_ALIGN_BYTE -
					(info_entry->cinfo.size % SCFS_CLUSTER_ALIGN_BYTE));
			}
#endif
		}
		mutex_unlock(&sii->cinfo_mutex);
	} else {
		/* for uncompressable files, we need at least one cinfo_entry in the list
			because write_meta depends on it, when determining whether to
			append with scfs_meta. */
		if (list_empty(&sii->cinfo_list)) {
			struct cinfo_entry *info_entry = NULL;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
			
			info_entry = scfs_alloc_cinfo_entry(PAGE_TO_CLUSTER_INDEX(page,sii),
					sii);
			sii->cluster_buffer.original_size = 0;
<<<<<<< HEAD
//			mutex_lock(&sbi->avail_space_mutex);
//			atomic_add(1, sbi->current_file_count);
//			mutex_unlock(&sbi->avail_space_mutex);
		}
		if (!PageUptodate(page)) {
			unsigned pos_in_page = pos & (PAGE_CACHE_SIZE - 1);
			//TODO: We should read exist page data from lower if page is not uptodate
=======
		}
		if (!PageUptodate(page)) {
			unsigned pos_in_page = pos & (PAGE_CACHE_SIZE - 1);
			//TODO: read existing page data from lower if page is not up-to-date
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
			if (pos_in_page) {
				char *source_addr;
				loff_t lower_pos;
			
<<<<<<< HEAD
				lower_pos = pos-pos_in_page;
=======
				lower_pos = pos - pos_in_page;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
				source_addr = (char*)kmap(page);
				ret = scfs_lower_read(lower_file, source_addr, pos_in_page, &lower_pos);
				if (ret < 0) {
					SCFS_PRINT_ERROR("read fail. ret = %d, size=%d\n", ret, len);
					lower_pos -= ret;
				} else
					ret = 0;
<<<<<<< HEAD
=======

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
				kunmap(page);
			}
		}
	}
	SetPageUptodate(page);
out:
	if (unlikely(ret)) {
		unlock_page(page);
		page_cache_release(page);
		*pagep = NULL;
	}

<<<<<<< HEAD
	SCFS_DEBUG_END;
=======
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	return ret;
}

extern struct kmem_cache *scfs_info_entry_list;
<<<<<<< HEAD
=======

/**
 * scfs_write_end
 * @file: The scfs file
 * @mapping: The file's address_space
 * @pos: The write starting position, in bytes
 * @len: Bytes to write in this page
 * @copied: Bytes actually written
 * @page: Page to be written
 * @fsdata: Fs-specific data (unused)
 *
 * Description:
 * - Finishes page write via scfs_lower_write. If the lower folder/partition
 *   is actually out of space and thus unable to satisfy the write request,
 *   write will fail by returning -ENOSPC.
 * - If a cluster is ready for write and compressable, then we compress it
 *   before writing it.
 * - Note that this will only finish writing the data given from the user, and
 *   SCFS metadata will be kept in-memory until scfs_put_lower_file (via fput())
 *   writes it down in a deferred manner.
 *
 */
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
static int scfs_write_end(struct file *file, struct address_space *mapping,
			loff_t pos, unsigned len, unsigned copied,
			struct page *page, void *fsdata)
{
	unsigned from = pos & (PAGE_CACHE_SIZE - 1);
	unsigned to = from + copied;
	struct scfs_inode_info *sii = SCFS_I(mapping->host);
	struct file *lower_file = NULL;
	struct scfs_sb_info *sb_info = SCFS_S(sii->vfs_inode.i_sb);
	loff_t lower_pos;
	int ret;
	struct cinfo_entry *info_entry = NULL;
<<<<<<< HEAD
 
	SCFS_PRINT("f:%s pos:%lld, len:%d s:%lld i:%d\n",
			file->f_path.dentry->d_name.name, pos, len, 
=======
	size_t tmp_len;

	SCFS_PRINT("f:%s pos:%lld, len:%d s:%lld i:%d\n",
			file->f_path.dentry->d_name.name, pos, copied, 
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
			i_size_read(&sii->vfs_inode), page->index);

	lower_file = scfs_lower_file(file);
	if (!lower_file) {
		SCFS_PRINT_ERROR("lower file is null!\n");
		ret = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	if (IS_COMPRESSED(sii)) {
=======
	if (IS_COMPRESSABLE(sii)) {
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 		ret = scfs_get_comp_buffer(sii);
		if (ret)
			goto out;

<<<<<<< HEAD
		sii->cluster_buffer.original_size += len; 

		atomic64_add(len, &sb_info->current_data_size);
		ret = scfs_check_space(sb_info, file->f_dentry);
		if(ret < 0) {
			atomic_sub(1, &sb_info->total_cluster_count);
			atomic64_sub(sii->cluster_buffer.original_size,&sb_info->current_data_size);
			SCFS_PRINT_ERROR("No more space in lower-storage\n");
			info_entry = list_entry(sii->cinfo_list.prev,
					struct cinfo_entry, entry);			
			list_del(&info_entry->entry);
			kmem_cache_free(scfs_info_entry_list, info_entry);
                    			info_entry = NULL;
			sii->cluster_buffer.original_size = 0;
			i_size_write(&sii->vfs_inode, 0);
			goto out;
		}
		//add data count will be written to sbi->count

=======
		sii->cluster_buffer.original_size += copied; 

		atomic64_add(copied, &sb_info->current_data_size);
		ret = scfs_check_space(sb_info, file->f_dentry);
		if(ret < 0) {
			SCFS_PRINT_ERROR("No more space in lower-storage\n");
			goto out;
		}
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

		sync_page_to_buffer(page, sii->cluster_buffer.u_buffer); 
 		if (PGOFF_IN_CLUSTER(page, sii) + 1 == sii->cluster_size / PAGE_CACHE_SIZE &&
				to == PAGE_CACHE_SIZE) {
<<<<<<< HEAD

			mutex_lock(&sii->cinfo_list_mutex);

			if (list_empty(&sii->cinfo_list)) {
				mutex_lock(&sii->cinfo_list_mutex);
=======
#ifdef SCFS_MULTI_THREAD_COMPRESSION
			struct scfs_cluster_buffer_mtc *cbm = kmem_cache_alloc(scfs_cbm_cache, GFP_KERNEL);
			if (!cbm) {
				SCFS_PRINT_ERROR("scfs_cluster_buffer_mtc alloc failed\n");
				ret = -ENOMEM;
				goto out;
			}
			memcpy(&cbm->entry, &sii->cluster_buffer, sizeof(struct scfs_cluster_buffer));
			cbm->is_compress_write_done = 0;

			while (sii->cbm_list_write_count > SMTC_PENDING_THRESHOLD)
				msleep(10);

			/* this lock is necessary? */
			mutex_lock(&sii->cinfo_mutex);
			if (list_empty(&sii->cinfo_list)) {
				mutex_unlock(&sii->cinfo_mutex);
				SCFS_PRINT_ERROR("cinfo list is empty\n");
				ret = -EINVAL;
				goto out;
			}
			info_entry = list_entry(sii->cinfo_list.prev,
					struct cinfo_entry, entry);

			if (info_entry->current_cluster_idx !=
					PAGE_TO_CLUSTER_INDEX(page,sii)) {
				SCFS_PRINT_ERROR("Cannot find cluster info entry" \
					"for cluster idx %d\n", PAGE_TO_CLUSTER_INDEX(page,sii));
				ASSERT(0);
			}
			mutex_unlock(&sii->cinfo_mutex);

			/* add this cluster_buffer to cbm_list.
			   Later, we should decrease current_data_size */
			spin_lock(&sb_info->sii_list_lock);
			list_add_tail(&cbm->list, &sii->cbm_list);
			SCFS_PRINT("cbm = 0x%08x pos = %d\n" , cbm, pos);

			cbm->info_entry = info_entry;
			sii->cbm_list_comp_count++;
			sb_info->cbm_list_total_count++;

			if (!sii->is_inserted_to_sii_list) {
				sii->is_inserted_to_sii_list = 1;
				list_add_tail(&sii->mtc_list, &sb_info->sii_list);
				sii->cbm_list_comp = &cbm->list;
				sii->cbm_list_write = &cbm->list;
			} else if (sii->cbm_list_comp_count == 1) {
				sii->cbm_list_comp = &cbm->list;
			}

			spin_unlock(&sb_info->sii_list_lock);
			wakeup_smtc_thread(sb_info);

			/* initialize sii->cluster_buffer */
			memset(&sii->cluster_buffer, 0, sizeof(struct scfs_cluster_buffer));
			atomic_sub(1, &sb_info->current_file_count);
#else
			mutex_lock(&sii->cinfo_mutex);

			if (list_empty(&sii->cinfo_list)) {
				mutex_unlock(&sii->cinfo_mutex);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
				SCFS_PRINT_ERROR("cinfo list is empty\n");
				ret = -EINVAL;
				goto out;
			}
			info_entry = list_entry(sii->cinfo_list.prev,
					struct cinfo_entry, entry);

			if (info_entry->current_cluster_idx !=
					PAGE_TO_CLUSTER_INDEX(page,sii)) {
				SCFS_PRINT_ERROR("Cannot find cluster info entry" \
					"for cluster idx %d\n", PAGE_TO_CLUSTER_INDEX(page,sii));
				ASSERT(0);
<<<<<<< HEAD
				//ret = -EINVAL;
				//goto out;
			}
			ret = scfs_compress(sii->comp_type, sii->cluster_buffer.c_buffer,
				sii->cluster_buffer.u_buffer,
				sii->cluster_buffer.original_size,
				&info_entry->cinfo.size);
			if (ret) {
				mutex_unlock(&sii->cinfo_list_mutex);
=======
			}
			/* Set cinfo size as available buffer size because zlib care about
			 * available buf size.  */
			info_entry->cinfo.size = PAGE_CACHE_SIZE*8;

			tmp_len = (size_t)info_entry->cinfo.size;
			ret = scfs_compress(sii->comp_type, sii->cluster_buffer.c_buffer,
				sii->cluster_buffer.u_buffer,
				sii->cluster_buffer.original_size,
				&tmp_len,
				NULL, sb_info);
			info_entry->cinfo.size = (__u32)(tmp_len & 0xffff);

			if (ret) {
				mutex_unlock(&sii->cinfo_mutex);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
				ClearPageUptodate(page);
				SCFS_PRINT_ERROR("compression failed.\n");
				goto out;
			}

			if (info_entry->cinfo.size >=
					sii->cluster_buffer.original_size *
					sb_info->options.comp_threshold / 100)
				info_entry->cinfo.size = sii->cluster_buffer.original_size;
	        	
<<<<<<< HEAD
			if (info_entry->cinfo.size % SCFS_CLUSTER_ALIGN)
				info_entry->pad = SCFS_CLUSTER_ALIGN -
					(info_entry->cinfo.size % SCFS_CLUSTER_ALIGN);
=======
			if (info_entry->cinfo.size % SCFS_CLUSTER_ALIGN_BYTE)
				info_entry->pad = SCFS_CLUSTER_ALIGN_BYTE -
					(info_entry->cinfo.size % SCFS_CLUSTER_ALIGN_BYTE);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
			else
				info_entry->pad = 0;

			SCFS_PRINT("cluster original size = %ld, comp size = %d, pad = %d\n"
				,sii->cluster_buffer.original_size
				,info_entry->cinfo.size
				,info_entry->pad);
				
			lower_pos = (loff_t)info_entry->cinfo.offset;

			if (info_entry->cinfo.size <
					sii->cluster_buffer.original_size *
					sb_info->options.comp_threshold / 100) {
				size_t write_count;
				write_count = (size_t)info_entry->cinfo.size+info_entry->pad;

<<<<<<< HEAD
				mutex_unlock(&sii->cinfo_list_mutex);
=======
				mutex_unlock(&sii->cinfo_mutex);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
				ret = scfs_lower_write(lower_file, sii->cluster_buffer.c_buffer,
					write_count, &lower_pos);
				if (ret < 0) {
					SCFS_PRINT_ERROR("write fail. ret = %d, size=%ld\n",
						ret, write_count);
					goto out;
				}

<<<<<<< HEAD
				if (!sii->compressd)
					sii->compressd = 1;
=======
				if (!sii->compressed)
					sii->compressed = 1;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
		    	} else {
				info_entry->cinfo.size = sii->cluster_buffer.original_size;
				info_entry->pad = 0;

<<<<<<< HEAD
				mutex_unlock(&sii->cinfo_list_mutex);
=======
				mutex_unlock(&sii->cinfo_mutex);

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
				ret = scfs_lower_write(lower_file, sii->cluster_buffer.u_buffer,
					sii->cluster_buffer.original_size, &lower_pos); 
				if (ret < 0) {
					SCFS_PRINT_ERROR("write fail. ret = %d, size=%d\n",
						ret, sii->cluster_buffer.original_size);
					goto out;
				}
<<<<<<< HEAD
	    	}	    	
			atomic64_sub(sii->cluster_buffer.original_size,&sb_info->current_data_size);
			sii->cluster_buffer.original_size = 0;
 		}
		ret = len;
=======
	    		}	    	
			atomic64_sub(sii->cluster_buffer.original_size,&sb_info->current_data_size);
			sii->cluster_buffer.original_size = 0;
#endif
 		}
		ret = copied;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	} else {
		char *source_addr;

		lower_pos = pos;
		source_addr = (char*)kmap(page);
<<<<<<< HEAD
		atomic64_add(len, &sb_info->current_data_size);
		ret = scfs_check_space(sb_info, file->f_dentry);
		if(ret < 0) {
			SCFS_PRINT_ERROR("No more space in lower-storage\n");
			goto out;
		}
		atomic64_sub(len, &sb_info->current_data_size);
		ret = scfs_lower_write(lower_file, source_addr+from, len, &lower_pos);
		if (ret < 0) {
			SCFS_PRINT_ERROR("write fail. ret = %d, size=%d\n", ret, len);
=======
		atomic64_add(copied, &sb_info->current_data_size);

		ret = scfs_check_space(sb_info, file->f_dentry);
		if (ret < 0) {
			SCFS_PRINT_ERROR("No more space in lower-storage\n");
			goto out;
		}
		atomic64_sub(copied, &sb_info->current_data_size);
		ret = scfs_lower_write(lower_file, source_addr+from, copied, &lower_pos);
		if (ret < 0) {
			SCFS_PRINT_ERROR("write fail. ret = %d, size=%d\n", ret, copied);
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
			goto out;
		}
		kunmap(page);
	}

	if (pos + copied > i_size_read(&sii->vfs_inode)) {
		i_size_write(&sii->vfs_inode, pos + copied);
		SCFS_PRINT("Expanded file size to [0x%.16llx]\n",
			(unsigned long long)i_size_read(&sii->vfs_inode));
	}
	SetPageUptodate(page);
out:
	unlock_page(page);
	page_cache_release(page);
<<<<<<< HEAD
	SCFS_DEBUG_END;
=======
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
	
	return ret;
}

<<<<<<< HEAD
=======
#ifdef SCFS_MULTI_THREAD_COMPRESSION
int smtc_init(struct scfs_sb_info *sbi)
{
	int i, j;

	for (i = 0; i < NR_CPUS; i++) {
		sbi->smtc_task[i] = kthread_run(smtc_thread, sbi, "scfs_mtc%d", i);

		if (IS_ERR(sbi->smtc_task[i])) {
			SCFS_PRINT_ERROR("smtc_init: creating kthread failed\n");

			for (j = 0; j < i; j++)
				kthread_stop(sbi->smtc_task[j]);
			return -ENOMEM;
		}
	}

	sbi->smtc_writer_task = kthread_run(smtc_writer_thread, sbi, "scfs_writer_mtc%d", 0);

	if (IS_ERR(sbi->smtc_writer_task)) {
		SCFS_PRINT_ERROR("smtc_init: creating kthread failed\n");
		return -ENOMEM;
	}
	return 0;
}

void smtc_destroy(struct scfs_sb_info *sbi)
{
	int i;

	for (i = 0 ; i < NR_CPUS ; i++) {
		if (sbi->smtc_task[i])
			kthread_stop(sbi->smtc_task[i]);
		sbi->smtc_task[i] = NULL;
	}

	if (sbi->smtc_writer_task)
		kthread_stop(sbi->smtc_writer_task);
	sbi->smtc_writer_task = NULL;
}

/* scaling # of threads will be woken up, on-demand */
void wakeup_smtc_thread(struct scfs_sb_info *sbi)
{
	u32 length;

	spin_lock(&sbi->sii_list_lock);
	length = sbi->cbm_list_total_count;
	spin_unlock(&sbi->sii_list_lock);

	if (length > 0 && sbi->smtc_task[0])
		wake_up_process(sbi->smtc_task[0]);
#if (NR_CPUS > 1)
	if (length >= SMTC_THREAD_THRESHOLD_2 && sbi->smtc_task[1])
		wake_up_process(sbi->smtc_task[1]);
#if (NR_CPUS > 2)
	if (length >= SMTC_THREAD_THRESHOLD_3 && sbi->smtc_task[2])
		wake_up_process(sbi->smtc_task[2]);
#if (NR_CPUS > 3)
	if (length >= SMTC_THREAD_THRESHOLD_4 && sbi->smtc_task[3])
		wake_up_process(sbi->smtc_task[3]);
#endif
#endif
#endif
}

void wakeup_smtc_writer_thread(struct scfs_sb_info *sbi)
{
	if (sbi->smtc_writer_task_status == 0 && sbi->smtc_writer_task)
		wake_up_process(sbi->smtc_writer_task);
}

int smtc_thread(void *info)
{
	struct scfs_sb_info *sbi = (struct scfs_sb_info *)info;
	struct scfs_inode_info *sii;
	struct scfs_cluster_buffer_mtc *cbm;
	int ret = 0;
	struct list_head *pos;
	void *workdata = NULL;

#ifndef CONFIG_SCFS_USE_CRYPTO
	int idx;

	switch (sbi->options.comp_type) {
	case SCFS_COMP_LZO:
		workdata = vmalloc(LZO1X_MEM_COMPRESS);
		idx = atomic_inc_return(&sbi->smtc_idx) - 1;
		sbi->smtc_workdata[idx] = workdata;
		if (!workdata) {
			SCFS_PRINT_ERROR("vmalloc for lzo workmem failed, "
					"len %d\n",
					LZO1X_MEM_COMPRESS);
			return -ENOMEM;
		} else {
			SCFS_PRINT("smtc workmem for lzo address : %p, idx : %d\n", workdata, idx);
		}
		break;
	default:
		break;
	}
#endif
	set_freezable();

	while (!kthread_should_stop()) {
		set_current_state(TASK_INTERRUPTIBLE);

		spin_lock(&sbi->sii_list_lock);

		if (sbi->cbm_list_total_count > 0) {
			list_for_each(pos, &sbi->sii_list) {
				sii = list_entry(pos, struct scfs_inode_info, mtc_list);
				if (sii->cbm_list_comp_count == 0) continue;
			}
			cbm = list_entry(sii->cbm_list_comp,
					struct scfs_cluster_buffer_mtc, list);
			sii->cbm_list_comp = sii->cbm_list_comp->next;
			sii->cbm_list_comp_count--;
			sii->cbm_list_write_count++;

			sbi->cbm_list_total_count--;
			SCFS_PRINT("(%d %d)\n", sbi->cbm_list_total_count,
				sii->cbm_list_comp_count);
			spin_unlock(&sbi->sii_list_lock);

			__set_current_state(TASK_RUNNING);

			ret = scfs_compress_cluster(sii, cbm, workdata);
			if (ret)
				SCFS_PRINT_ERROR("compress failed. ret = %d\n", ret);			
		} else {
			spin_unlock(&sbi->sii_list_lock);
			schedule();
		}
	}
#ifndef CONFIG_SCFS_USE_CRYPTO
	if (workdata)
		vfree(workdata);
#endif
	return 0;
}

int smtc_writer_thread(void *info)
{
	struct scfs_sb_info *sbi = (struct scfs_sb_info *)info;
	struct scfs_inode_info *sii;
	struct scfs_cluster_buffer_mtc *cbm;
	int ret = 0;
	struct list_head *pos, *pos1;

	set_freezable();
	while (!kthread_should_stop()) {
		__set_current_state(TASK_RUNNING);

		spin_lock(&sbi->sii_list_lock);
		list_for_each(pos, &sbi->sii_list) {
			sii = list_entry(pos, struct scfs_inode_info, mtc_list);

			for (pos1 = sii->cbm_list_write; pos1 != &sii->cbm_list;) {
				cbm = list_entry(pos1, struct scfs_cluster_buffer_mtc, list);

				if (cbm->is_compress_write_done == 0)
					break;
				if (cbm->is_compress_write_done == 2) {
					pos1 = pos1->next;
					list_del(&cbm->list);
					kmem_cache_free(scfs_cbm_cache, cbm);
					continue;
				}
				sii->cbm_list_write_count--;
				spin_unlock(&sbi->sii_list_lock);

				BUG_ON(cbm->is_compress_write_done != 1);

				/* do lower buffered-write for this cbm */
				ret = scfs_write_one_compress_cluster(sii, cbm);
				if (ret < 0) {
					SCFS_PRINT_ERROR("lower buffered-write failed.\n");
					break;
				}
				spin_lock(&sbi->sii_list_lock);
				pos1 = pos1->next;
			}

			if (pos1 == &sii->cbm_list)
				sii->cbm_list_write = pos1->next;
			else
				sii->cbm_list_write = pos1;
		}
		spin_unlock(&sbi->sii_list_lock);
		set_current_state(TASK_INTERRUPTIBLE);
		sbi->smtc_writer_task_status = 0;
		schedule();
		sbi->smtc_writer_task_status = 1;
	}
	return 0;
}

int scfs_compress_cluster(struct scfs_inode_info *sii,
	struct scfs_cluster_buffer_mtc *cbm, void *workdata)
{
	struct scfs_sb_info *sb_info = SCFS_S(sii->vfs_inode.i_sb);
	struct cinfo_entry *info_entry;
	int ret = 0;
	struct scfs_cluster_buffer *cb;
	size_t tmp_len;

	info_entry = cbm->info_entry;
	cb = &cbm->entry;

	tmp_len = (size_t)info_entry->cinfo.size;
	ret = scfs_compress(sii->comp_type, cb->c_buffer, cb->u_buffer,
		cb->original_size, &tmp_len, workdata, sb_info);
	info_entry->cinfo.size = (__u32)(tmp_len & 0xffff);
	if (ret) {
		SCFS_PRINT_ERROR("compression failed.\n");
		goto out;
	}

	if (info_entry->cinfo.size >=
		cb->original_size *
		sb_info->options.comp_threshold / 100) {
		info_entry->cinfo.size = cb->original_size;
		//Free invalid pages
		__free_pages(cb->c_page, SCFS_MEMPOOL_ORDER + 1);
	} else {
		//Free invalid pages
		__free_pages(cb->u_page, SCFS_MEMPOOL_ORDER + 1);	
	}
    
	if (info_entry->cinfo.size % SCFS_CLUSTER_ALIGN_BYTE)
		info_entry->pad = SCFS_CLUSTER_ALIGN_BYTE -
			(info_entry->cinfo.size % SCFS_CLUSTER_ALIGN_BYTE);
	else
		info_entry->pad = 0;

	spin_lock(&sb_info->sii_list_lock);
	cbm->is_compress_write_done = 1;
	spin_unlock(&sb_info->sii_list_lock);

	SCFS_PRINT("cbm = 0x%08x comp = %d\n" , cbm, info_entry->cinfo.size);

	wakeup_smtc_writer_thread(sb_info);

out:
	return ret;
}

int scfs_write_one_compress_cluster(struct scfs_inode_info *sii,
	struct scfs_cluster_buffer_mtc *cbm)
{
	struct scfs_sb_info *sbi = SCFS_S(sii->vfs_inode.i_sb);
	struct cinfo_entry *info_entry;
	struct cinfo_entry *prev_info_entry;
	loff_t lower_pos;
	int ret = 0;
	struct file *lower_file = sii->lower_file;
	struct scfs_cluster_buffer *cb;

	info_entry = cbm->info_entry;
	cb = &cbm->entry;

	if (info_entry->cinfo.size >= cb->original_size *
		sbi->options.comp_threshold / 100)
		info_entry->cinfo.size = cb->original_size;
    
	if (info_entry->cinfo.size % SCFS_CLUSTER_ALIGN_BYTE)
		info_entry->pad = SCFS_CLUSTER_ALIGN_BYTE -
			(info_entry->cinfo.size % SCFS_CLUSTER_ALIGN_BYTE);
	else
		info_entry->pad = 0;

	/* update current cluster's offset using previous cluster */
	if (info_entry->entry.prev != &sii->cinfo_list) {
		prev_info_entry = list_entry(info_entry->entry.prev, struct cinfo_entry, entry);
		info_entry->cinfo.offset = prev_info_entry->cinfo.offset +
			prev_info_entry->cinfo.size;

		if (prev_info_entry->cinfo.size % SCFS_CLUSTER_ALIGN_BYTE)
			info_entry->cinfo.offset += (SCFS_CLUSTER_ALIGN_BYTE -
				(prev_info_entry->cinfo.size % SCFS_CLUSTER_ALIGN_BYTE));
	}

	lower_pos = (loff_t)info_entry->cinfo.offset;

	if (info_entry->cinfo.size < cb->original_size *
			sbi->options.comp_threshold / 100) {
		size_t write_count;

		write_count = (size_t)info_entry->cinfo.size + info_entry->pad;

		ret = scfs_lower_write(lower_file, cb->c_buffer, write_count, &lower_pos);
		if (ret < 0) {
			SCFS_PRINT_ERROR("write fail. ret = %d, size=%ld\n",
				ret, write_count);
			goto out;
		}

		if (!sii->compressed)
			sii->compressed = 1;

		__free_pages(cb->c_page, SCFS_MEMPOOL_ORDER + 1);
    	} else {
		info_entry->cinfo.size = cb->original_size;
		info_entry->pad = 0;

		ret = scfs_lower_write(lower_file, cb->u_buffer, info_entry->cinfo.size, &lower_pos); 
		if (ret < 0) {
			SCFS_PRINT_ERROR("write fail. ret = %d, size=%d\n",
				ret, cb->original_size);
			goto out;
		}
		__free_pages(cb->u_page, SCFS_MEMPOOL_ORDER + 1);
	}	    	
	atomic64_sub(cb->original_size, &sbi->current_data_size);

	spin_lock(&sbi->sii_list_lock);
	cbm->is_compress_write_done = 2;
	spin_unlock(&sbi->sii_list_lock);

	/* free the pages for this cbm */
//	__free_pages(cb->u_page, SCFS_MEMPOOL_ORDER + 1);
//	__free_pages(cb->c_page, SCFS_MEMPOOL_ORDER + 1);
//	atomic64_sub(PAGE_SIZE*32,&sbi->memory_footprint);
//	kmem_cache_free(sbi->scfs_cbm_cache, cbm);

out:
	return ret;
}

int scfs_write_compress_all_cluster(struct scfs_inode_info *sii, struct file *lower_file)
{
	struct scfs_sb_info *sbi = SCFS_S(sii->vfs_inode.i_sb);
	struct scfs_cluster_buffer_mtc *cbm;
	struct cinfo_entry *info_entry;
	loff_t lower_pos;
	int ret = 0;
	struct list_head *pos;
	struct scfs_cluster_buffer *cb;

	spin_lock(&sbi->sii_list_lock);

	if (!sii->is_inserted_to_sii_list) {
		spin_unlock(&sbi->sii_list_lock);
		goto out;
	}

	while (sii->cbm_list_comp_count) {
		spin_unlock(&sbi->sii_list_lock);
		wakeup_smtc_thread(sbi);
		msleep(100);
		spin_lock(&sbi->sii_list_lock);
	}

	while (sii->cbm_list_write_count) {
		spin_unlock(&sbi->sii_list_lock);
		wakeup_smtc_writer_thread(sbi);
		msleep(20);
		spin_lock(&sbi->sii_list_lock);
	}
	spin_unlock(&sbi->sii_list_lock);

	while (sbi->smtc_writer_task_status == 1)
		msleep(20);

	list_for_each(pos, &sii->cbm_list) {
		BUG_ON(sii->cbm_list_comp_count);
		cbm = list_entry(pos, struct scfs_cluster_buffer_mtc, list);
		spin_lock(&sbi->sii_list_lock);

		if (cbm->is_compress_write_done == 2) {
			spin_unlock(&sbi->sii_list_lock);
			kmem_cache_free(scfs_cbm_cache, cbm);
			continue;
		}
		spin_unlock(&sbi->sii_list_lock);

		SCFS_PRINT_ALWAYS("0x%08x 0x%08x 0x%08x 0x%08x %d 0x%08x 0x%08x %d\n",
			cbm->entry.c_page, cbm->entry.u_page, cbm->entry.c_buffer,
			cbm->entry.u_buffer, cbm->entry.original_size,
			cbm->info_entry, &cbm->list, cbm->is_compress_write_done);

		BUG_ON(cbm->is_compress_write_done != 1);

		info_entry = cbm->info_entry;
		cb = &cbm->entry;

		if (info_entry->cinfo.size >= cb->original_size *
			sbi->options.comp_threshold / 100)
			info_entry->cinfo.size = cb->original_size;
        	
		if (info_entry->cinfo.size % SCFS_CLUSTER_ALIGN_BYTE)
			info_entry->pad = SCFS_CLUSTER_ALIGN_BYTE -
				(info_entry->cinfo.size % SCFS_CLUSTER_ALIGN_BYTE);
		else
			info_entry->pad = 0;

		SCFS_PRINT("comp = %d\n" , info_entry->cinfo.size);
			
		lower_pos = (loff_t)info_entry->cinfo.offset;

		if (info_entry->cinfo.size < cb->original_size *
				sbi->options.comp_threshold / 100) {
			size_t write_count;

			write_count = (size_t)info_entry->cinfo.size+info_entry->pad;

			ret = scfs_lower_write(lower_file, cb->c_buffer, write_count, &lower_pos);
			if (ret < 0) {
				SCFS_PRINT_ERROR("write fail. ret = %d, size=%ld\n",
					ret, write_count);
				goto out;
			}

			if (!sii->compressed)
				sii->compressed = 1;
	    	} else {
			info_entry->cinfo.size = cb->original_size;
			info_entry->pad = 0;

			ret = scfs_lower_write(lower_file, cb->u_buffer,
				cb->original_size, &lower_pos); 
			if (ret < 0) {
				SCFS_PRINT_ERROR("write fail. ret = %d, size=%d\n",
					ret, cb->original_size);
				goto out;
			}
    		}	    	
		atomic64_sub(cb->original_size, &sbi->current_data_size);

		/* clear this cbm */
		__free_pages(cb->u_page, SCFS_MEMPOOL_ORDER + 1);
		__free_pages(cb->c_page, SCFS_MEMPOOL_ORDER + 1);
		kmem_cache_free(scfs_cbm_cache, cbm);
	}

	if (sii->is_inserted_to_sii_list) {
		list_del(&sii->mtc_list);

		INIT_LIST_HEAD(&sii->cbm_list);
		sii->cbm_list_comp_count = 0;
		sii->cbm_list_write_count = 0;
		//sii->is_inserted_to_sii_list = 0;
		sii->cbm_list_comp = NULL;
		//SCFS_PRINT_ALWAYS("end\n");
	}
out:
	return ret;
}
#endif

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
/**************************************/
/* address_space_operations structure */
/**************************************/

const struct address_space_operations scfs_aops = {
	.readpage = scfs_readpage,
#ifdef SCFS_ASYNC_READ_PAGES
	.readpages = scfs_readpages,
#endif
	.write_begin = scfs_write_begin,
	.write_end = scfs_write_end,
};
