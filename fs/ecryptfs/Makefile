#
# Makefile for the Linux 2.6 eCryptfs
#

<<<<<<< HEAD
obj-$(CONFIG_ECRYPT_FS) += ecryptfs.o
=======
ifeq ($(CONFIG_ECRYPT_FS_VIRTUAL_FAT_XATTR),y)
EXTRA_CFLAGS += -DECRYPT_FS_VIRTUAL_FAT_XATTR
endif

obj-$(CONFIG_ECRYPT_FS) += ecryptfs.o
obj-$(CONFIG_SDP) += ecryptfs_dek.o mm.o ecryptfs_sdp_chamber.o
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

ecryptfs-objs := dentry.o file.o inode.o main.o super.o mmap.o read_write.o crypto.o keystore.o messaging.o miscdev.o kthread.o debug.o
