# Makefile for the Linux device tree

obj-y			:= core.o bus.o dd.o syscore.o \
			   driver.o class.o platform.o \
			   cpu.o firmware.o init.o map.o devres.o \
			   attribute_container.o transport_class.o \
<<<<<<< HEAD
			   topology.o sys.o
obj-$(CONFIG_DEVTMPFS)	+= devtmpfs.o
obj-$(CONFIG_CMA) += dma-contiguous.o
=======
			   topology.o
obj-$(CONFIG_DEVTMPFS)	+= devtmpfs.o
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
obj-y			+= power/
obj-$(CONFIG_HAS_DMA)	+= dma-mapping.o
obj-$(CONFIG_HAVE_GENERIC_DMA_COHERENT) += dma-coherent.o
obj-$(CONFIG_DMA_SHARED_BUFFER) += dma-buf.o
<<<<<<< HEAD
obj-$(CONFIG_GENLOCK) += genlock.o
=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
obj-$(CONFIG_ISA)	+= isa.o
obj-$(CONFIG_FW_LOADER)	+= firmware_class.o
obj-$(CONFIG_NUMA)	+= node.o
obj-$(CONFIG_MEMORY_HOTPLUG_SPARSE) += memory.o
ifeq ($(CONFIG_SYSFS),y)
obj-$(CONFIG_MODULES)	+= module.o
endif
obj-$(CONFIG_SYS_HYPERVISOR) += hypervisor.o
obj-$(CONFIG_REGMAP)	+= regmap/
obj-$(CONFIG_SOC_BUS) += soc.o
<<<<<<< HEAD
obj-$(CONFIG_PINCTRL) += pinctrl.o

obj-$(CONFIG_SYNC)	+= sync.o
obj-$(CONFIG_SW_SYNC)	+= sw_sync.o
=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4

ccflags-$(CONFIG_DEBUG_DRIVER) := -DDEBUG

