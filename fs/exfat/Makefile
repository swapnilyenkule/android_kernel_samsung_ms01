<<<<<<< HEAD
=======

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
obj-y += exfat_core.o exfat_fs.o

exfat_fs-y	:= exfat_super.o

exfat_core-y	:= exfat.o exfat_api.o exfat_blkdev.o exfat_cache.o \
<<<<<<< HEAD
			   exfat_data.o exfat_global.o exfat_nls.o exfat_oal.o exfat_upcase.o
=======
			   exfat_data.o exfat_global.o exfat_nls.o \
			   exfat_oal.o exfat_upcase.o exfat_xattr.o
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	
