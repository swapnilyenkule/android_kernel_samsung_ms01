<<<<<<< HEAD
=======
ifeq ($(CONFIG_FIPS_ENABLE), y)
	obj-$(CONFIG_CRYPTO_DEV_QCEDEV) += qcedev_fips.o
endif
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
obj-$(CONFIG_CRYPTO_DEV_QCEDEV) += qcedev.o
ifeq ($(CONFIG_CRYPTO_DEV_QCE50), y)
	obj-$(CONFIG_CRYPTO_DEV_QCE) += qce50.o
else
	ifeq ($(CONFIG_CRYPTO_DEV_QCE40), y)
		obj-$(CONFIG_CRYPTO_DEV_QCE) += qce40.o
	else
		obj-$(CONFIG_CRYPTO_DEV_QCE) += qce.o
	endif
endif
<<<<<<< HEAD
=======
ifeq ($(CONFIG_FIPS_ENABLE), y)
	obj-$(CONFIG_CRYPTO_DEV_QCRYPTO) += qcrypto_fips.o
endif
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
obj-$(CONFIG_CRYPTO_DEV_QCRYPTO) += qcrypto.o
obj-$(CONFIG_CRYPTO_DEV_OTA_CRYPTO) += ota_crypto.o
