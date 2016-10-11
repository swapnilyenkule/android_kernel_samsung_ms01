<<<<<<< HEAD

=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
/*
 *  linux/mm/page_io.c
 *
 *  Copyright (C) 1991, 1992, 1993, 1994  Linus Torvalds
 *
 *  Swap reorganised 29.12.95, 
 *  Asynchronous swapping added 30.12.95. Stephen Tweedie
 *  Removed race in async swapping. 14.4.1996. Bruno Haible
 *  Add swap of shared pages through the page cache. 20.2.1998. Stephen Tweedie
 *  Always use brw_page, life becomes simpler. 12 May 1998 Eric Biederman
 */

#include <linux/mm.h>
#include <linux/kernel_stat.h>
#include <linux/gfp.h>
#include <linux/pagemap.h>
#include <linux/swap.h>
#include <linux/bio.h>
#include <linux/swapops.h>
#include <linux/writeback.h>
<<<<<<< HEAD
#include <linux/blkdev.h>
#include <linux/frontswap.h>
=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
#include <asm/pgtable.h>

static struct bio *get_swap_bio(gfp_t gfp_flags,
				struct page *page, bio_end_io_t end_io)
{
	struct bio *bio;

	bio = bio_alloc(gfp_flags, 1);
	if (bio) {
		bio->bi_sector = map_swap_page(page, &bio->bi_bdev);
		bio->bi_sector <<= PAGE_SHIFT - 9;
		bio->bi_io_vec[0].bv_page = page;
		bio->bi_io_vec[0].bv_len = PAGE_SIZE;
		bio->bi_io_vec[0].bv_offset = 0;
		bio->bi_vcnt = 1;
		bio->bi_idx = 0;
		bio->bi_size = PAGE_SIZE;
		bio->bi_end_io = end_io;
	}
	return bio;
}

<<<<<<< HEAD
void end_swap_bio_write(struct bio *bio, int err)
=======
static void end_swap_bio_write(struct bio *bio, int err)
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
{
	const int uptodate = test_bit(BIO_UPTODATE, &bio->bi_flags);
	struct page *page = bio->bi_io_vec[0].bv_page;

	if (!uptodate) {
		SetPageError(page);
		/*
		 * We failed to write the page out to swap-space.
		 * Re-dirty the page in order to avoid it being reclaimed.
		 * Also print a dire warning that things will go BAD (tm)
		 * very quickly.
		 *
		 * Also clear PG_reclaim to avoid rotate_reclaimable_page()
		 */
		set_page_dirty(page);
<<<<<<< HEAD
#ifndef CONFIG_VNSWAP
=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
		printk(KERN_ALERT "Write-error on swap-device (%u:%u:%Lu)\n",
				imajor(bio->bi_bdev->bd_inode),
				iminor(bio->bi_bdev->bd_inode),
				(unsigned long long)bio->bi_sector);
<<<<<<< HEAD
#endif
=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
		ClearPageReclaim(page);
	}
	end_page_writeback(page);
	bio_put(bio);
}

void end_swap_bio_read(struct bio *bio, int err)
{
	const int uptodate = test_bit(BIO_UPTODATE, &bio->bi_flags);
	struct page *page = bio->bi_io_vec[0].bv_page;

	if (!uptodate) {
		SetPageError(page);
		ClearPageUptodate(page);
		printk(KERN_ALERT "Read-error on swap-device (%u:%u:%Lu)\n",
				imajor(bio->bi_bdev->bd_inode),
				iminor(bio->bi_bdev->bd_inode),
				(unsigned long long)bio->bi_sector);
<<<<<<< HEAD
		goto out;
	}

	SetPageUptodate(page);

	/*
	 * There is no guarantee that the page is in swap cache - the software
	 * suspend code (at least) uses end_swap_bio_read() against a non-
	 * swapcache page.  So we must check PG_swapcache before proceeding with
	 * this optimization.
	 */
	if (likely(PageSwapCache(page))) {
		struct swap_info_struct *sis;

		sis = page_swap_info(page);
		if (sis->flags & SWP_BLKDEV) {
			/*
			 * The swap subsystem performs lazy swap slot freeing,
			 * expecting that the page will be swapped out again.
			 * So we can avoid an unnecessary write if the page
			 * isn't redirtied.
			 * This is good for real swap storage because we can
			 * reduce unnecessary I/O and enhance wear-leveling
			 * if an SSD is used as the as swap device.
			 * But if in-memory swap device (eg zram) is used,
			 * this causes a duplicated copy between uncompressed
			 * data in VM-owned memory and compressed data in
			 * zram-owned memory.  So let's free zram-owned memory
			 * and make the VM-owned decompressed page *dirty*,
			 * so the page should be swapped out somewhere again if
			 * we again wish to reclaim it.
			 */
			struct gendisk *disk = sis->bdev->bd_disk;
			if (disk->fops->swap_slot_free_notify) {
				swp_entry_t entry;
				unsigned long offset;

				entry.val = page_private(page);
				offset = swp_offset(entry);

				SetPageDirty(page);
				disk->fops->swap_slot_free_notify(sis->bdev,
						offset);
			}
		}
	}

out:
=======
	} else {
		SetPageUptodate(page);
	}
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	unlock_page(page);
	bio_put(bio);
}

<<<<<<< HEAD
int __swap_writepage(struct page *page, struct writeback_control *wbc,
	void (*end_write_func)(struct bio *, int));

=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
/*
 * We may have stale swap cache pages in memory: notice
 * them here and get rid of the unnecessary final write.
 */
int swap_writepage(struct page *page, struct writeback_control *wbc)
{
<<<<<<< HEAD
	int ret = 0;
=======
	struct bio *bio;
	int ret = 0, rw = WRITE;
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4

	if (try_to_free_swap(page)) {
		unlock_page(page);
		goto out;
	}
<<<<<<< HEAD
	if (frontswap_store(page) == 0) {
		set_page_writeback(page);
		unlock_page(page);
		end_page_writeback(page);
		goto out;
	}
	ret = __swap_writepage(page, wbc, end_swap_bio_write);
out:
	return ret;
}

int __swap_writepage(struct page *page, struct writeback_control *wbc,
	void (*end_write_func)(struct bio *, int))
{
	struct bio *bio;
	int ret = 0, rw = WRITE;

	bio = get_swap_bio(GFP_NOIO, page, end_write_func);
=======
	bio = get_swap_bio(GFP_NOIO, page, end_swap_bio_write);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	if (bio == NULL) {
		set_page_dirty(page);
		unlock_page(page);
		ret = -ENOMEM;
		goto out;
	}
	if (wbc->sync_mode == WB_SYNC_ALL)
		rw |= REQ_SYNC;
	count_vm_event(PSWPOUT);
	set_page_writeback(page);
	unlock_page(page);
	submit_bio(rw, bio);
out:
	return ret;
}

int swap_readpage(struct page *page)
{
	struct bio *bio;
	int ret = 0;

	VM_BUG_ON(!PageLocked(page));
	VM_BUG_ON(PageUptodate(page));
<<<<<<< HEAD
	if (frontswap_load(page) == 0) {
		SetPageUptodate(page);
		unlock_page(page);
		goto out;
	}
=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	bio = get_swap_bio(GFP_KERNEL, page, end_swap_bio_read);
	if (bio == NULL) {
		unlock_page(page);
		ret = -ENOMEM;
		goto out;
	}
	count_vm_event(PSWPIN);
	submit_bio(READ, bio);
out:
	return ret;
}
