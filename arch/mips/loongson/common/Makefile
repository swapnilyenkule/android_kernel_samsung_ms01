#
# Makefile for loongson based machines.
#

obj-y += setup.o init.o cmdline.o env.o time.o reset.o irq.o \
    pci.o bonito-irq.o mem.o machtype.o platform.o
obj-$(CONFIG_GENERIC_GPIO) += gpio.o

#
# Serial port support
#
obj-$(CONFIG_EARLY_PRINTK) += early_printk.o
<<<<<<< HEAD
obj-$(CONFIG_SERIAL_8250) += serial.o
=======
loongson-serial-$(CONFIG_SERIAL_8250) := serial.o
obj-y += $(loongson-serial-m) $(loongson-serial-y)
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
obj-$(CONFIG_LOONGSON_UART_BASE) += uart_base.o
obj-$(CONFIG_LOONGSON_MC146818) += rtc.o

#
# Enable CS5536 Virtual Support Module(VSM) to virtulize the PCI configure
# space
#
obj-$(CONFIG_CS5536) += cs5536/

#
# Suspend Support
#

obj-$(CONFIG_LOONGSON_SUSPEND) += pm.o
