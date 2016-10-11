/*
 * various internal comedi functions
 */
int do_rangeinfo_ioctl(struct comedi_device *dev,
		       struct comedi_rangeinfo __user *arg);
int insn_inval(struct comedi_device *dev, struct comedi_subdevice *s,
	       struct comedi_insn *insn, unsigned int *data);
int comedi_alloc_board_minor(struct device *hardware_device);
void comedi_free_board_minor(unsigned minor);
<<<<<<< HEAD
=======
int comedi_find_board_minor(struct device *hardware_device);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
void comedi_reset_async_buf(struct comedi_async *async);
int comedi_buf_alloc(struct comedi_device *dev, struct comedi_subdevice *s,
		     unsigned long new_size);
