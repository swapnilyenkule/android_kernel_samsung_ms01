#
# Makefile for the kernel multimedia device drivers.
#

media-objs	:= media-device.o media-devnode.o media-entity.o

ifeq ($(CONFIG_MEDIA_CONTROLLER),y)
  obj-$(CONFIG_MEDIA_SUPPORT) += media.o
endif

<<<<<<< HEAD
obj-y += common/ platform/ rc/ video/

obj-$(CONFIG_VIDEO_DEV) += radio/
obj-$(CONFIG_DVB_CORE)  += dvb/
obj-$(CONFIG_TDMB)  += tdmb/
obj-$(CONFIG_ISDBTMM)   += isdbtmm/
obj-$(CONFIG_ISDBT) += isdbt/
obj-$(CONFIG_MTV) += mtv/
=======
obj-y += common/ rc/ video/

obj-$(CONFIG_VIDEO_DEV) += radio/
obj-$(CONFIG_DVB_CORE)  += dvb/
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
