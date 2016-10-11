#ifndef __LINUX_PAGEISOLATION_H
#define __LINUX_PAGEISOLATION_H

<<<<<<< HEAD
int move_freepages(struct zone *zone,
			  struct page *start_page, struct page *end_page,
			  int migratetype);

/*
 * Changes migrate type in [start_pfn, end_pfn) to be MIGRATE_ISOLATE.
 * If specified range includes migrate types other than MOVABLE or CMA,
=======
/*
 * Changes migrate type in [start_pfn, end_pfn) to be MIGRATE_ISOLATE.
 * If specified range includes migrate types other than MOVABLE,
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
 * this will fail with -EBUSY.
 *
 * For isolating all pages in the range finally, the caller have to
 * free all pages in the range. test_page_isolated() can be used for
 * test it.
 */
extern int
<<<<<<< HEAD
start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			 unsigned migratetype);
=======
start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4

/*
 * Changes MIGRATE_ISOLATE to MIGRATE_MOVABLE.
 * target range is [start_pfn, end_pfn)
 */
extern int
<<<<<<< HEAD
undo_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			unsigned migratetype);

/*
 * Test all pages in [start_pfn, end_pfn) are isolated or not.
 */
int test_pages_isolated(unsigned long start_pfn, unsigned long end_pfn);

/*
 * Internal functions. Changes pageblock's migrate type.
 */
extern int set_migratetype_isolate(struct page *page);
extern void unset_migratetype_isolate(struct page *page, unsigned migratetype);
=======
undo_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn);

/*
 * test all pages in [start_pfn, end_pfn)are isolated or not.
 */
extern int
test_pages_isolated(unsigned long start_pfn, unsigned long end_pfn);

/*
 * Internal funcs.Changes pageblock's migrate type.
 * Please use make_pagetype_isolated()/make_pagetype_movable().
 */
extern int set_migratetype_isolate(struct page *page);
extern void unset_migratetype_isolate(struct page *page);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4


#endif
