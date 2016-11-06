TARGET_NAME := mwlan_aarp
TARGET_KO := $(TARGET_NAME).ko

CLIB := .

obj-m += $(TARGET_NAME).o
$(TARGET_NAME)-objs := \
	a.o\

<<<<<<< HEAD
EXTRA_CFLAGS += -Wall -I $(PWD)/$(CLIB)
EXTRA_CFLAGS += $(_CFLAGS)
=======
EXTRA_CFLAGS += -Wall -Wno-uninitialized -I $(PWD)/$(CLIB)
EXTRA_CFLAGS += $(_CFLAGS)
EXTRA_CFLAGS += -DMODEL_$(subst -,_,$(PRODUCT_MODEL))
# only over the Android 5.0
EXTRA_CFLAGS += -DPLAT_VER=$(shell printf 0x50000)
# end
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

all:
	make -C $(SRCPATH) M=$(PWD) modules

clean:
	make -C $(SRCPATH) M=$(PWD) clean
