/*
 * Arch specific extensions to struct device
 *
 * This file is released under the GPLv2
 */
#ifndef ASMARM_DEVICE_H
#define ASMARM_DEVICE_H

struct dev_archdata {
<<<<<<< HEAD
	struct dma_map_ops	*dma_ops;
=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
#ifdef CONFIG_DMABOUNCE
	struct dmabounce_device_info *dmabounce;
#endif
#ifdef CONFIG_IOMMU_API
	void *iommu; /* private IOMMU data */
#endif
<<<<<<< HEAD
#ifdef CONFIG_ARM_DMA_USE_IOMMU
	struct dma_iommu_mapping	*mapping;
#endif
=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
};

struct omap_device;

struct pdev_archdata {
#ifdef CONFIG_ARCH_OMAP
	struct omap_device *od;
#endif
};

#endif
