<<<<<<< HEAD
SDCARDFS_VERSION="0.1"

EXTRA_CFLAGS += -DSDCARDFS_VERSION=\"$(SDCARDFS_VERSION)\"
=======
ifeq ($(CONFIG_SDCARD_FS_XATTR),y)
EXTRA_CFLAGS += -DSDCARD_FS_XATTR
endif
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

obj-$(CONFIG_SDCARD_FS) += sdcardfs.o

sdcardfs-y := dentry.o file.o inode.o main.o super.o lookup.o mmap.o packagelist.o derived_perm.o
<<<<<<< HEAD
=======
sdcardfs-$(CONFIG_SDCARD_FS_XATTR) += xattr.o
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
