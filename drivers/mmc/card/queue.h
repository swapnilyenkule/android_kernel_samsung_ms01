#ifndef MMC_QUEUE_H
#define MMC_QUEUE_H

struct request;
struct task_struct;

struct mmc_blk_request {
	struct mmc_request	mrq;
	struct mmc_command	sbc;
	struct mmc_command	cmd;
	struct mmc_command	stop;
	struct mmc_data		data;
};

<<<<<<< HEAD
enum mmc_packed_cmd {
	MMC_PACKED_NONE = 0,
	MMC_PACKED_WRITE,
};

=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
struct mmc_queue_req {
	struct request		*req;
	struct mmc_blk_request	brq;
	struct scatterlist	*sg;
	char			*bounce_buf;
	struct scatterlist	*bounce_sg;
	unsigned int		bounce_sg_len;
	struct mmc_async_req	mmc_active;
<<<<<<< HEAD
	struct list_head	packed_list;
	u32			packed_cmd_hdr[128];
	unsigned int		packed_blocks;
	enum mmc_packed_cmd	packed_cmd;
	int		packed_retries;
	int		packed_fail_idx;
	u8		packed_num;
=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
};

struct mmc_queue {
	struct mmc_card		*card;
	struct task_struct	*thread;
	struct semaphore	thread_sem;
	unsigned int		flags;
<<<<<<< HEAD
#define MMC_QUEUE_SUSPENDED		(1 << 0)
#define MMC_QUEUE_NEW_REQUEST		(1 << 1)
#define MMC_QUEUE_URGENT_REQUEST	(1 << 2)

=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	int			(*issue_fn)(struct mmc_queue *, struct request *);
	void			*data;
	struct request_queue	*queue;
	struct mmc_queue_req	mqrq[2];
	struct mmc_queue_req	*mqrq_cur;
	struct mmc_queue_req	*mqrq_prev;
<<<<<<< HEAD
	bool			wr_packing_enabled;
	int			num_of_potential_packed_wr_reqs;
	int			num_wr_reqs_to_start_packing;
	bool			no_pack_for_random;
	int (*err_check_fn) (struct mmc_card *, struct mmc_async_req *);
	void (*packed_test_fn) (struct request_queue *, struct mmc_queue_req *);
=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
};

extern int mmc_init_queue(struct mmc_queue *, struct mmc_card *, spinlock_t *,
			  const char *);
extern void mmc_cleanup_queue(struct mmc_queue *);
<<<<<<< HEAD
extern int mmc_queue_suspend(struct mmc_queue *, int);
=======
extern void mmc_queue_suspend(struct mmc_queue *);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
extern void mmc_queue_resume(struct mmc_queue *);

extern unsigned int mmc_queue_map_sg(struct mmc_queue *,
				     struct mmc_queue_req *);
extern void mmc_queue_bounce_pre(struct mmc_queue_req *);
extern void mmc_queue_bounce_post(struct mmc_queue_req *);

<<<<<<< HEAD
extern void print_mmc_packing_stats(struct mmc_card *card);

=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
#endif
