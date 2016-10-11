/**
 * host.c - DesignWare USB3 DRD Controller Host Glue
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com
 *
 * Authors: Felipe Balbi <balbi@ti.com>,
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the above-listed copyright holders may not be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * ALTERNATIVELY, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2, as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/platform_device.h>

#include "core.h"
<<<<<<< HEAD
#include "xhci.h"
=======

static struct resource generic_resources[] = {
	{
		.flags = IORESOURCE_IRQ,
	},
	{
		.flags = IORESOURCE_MEM,
	},
};
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4

int dwc3_host_init(struct dwc3 *dwc)
{
	struct platform_device	*xhci;
	int			ret;
<<<<<<< HEAD
	struct xhci_plat_data	pdata;
=======
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4

	xhci = platform_device_alloc("xhci-hcd", -1);
	if (!xhci) {
		dev_err(dwc->dev, "couldn't allocate xHCI device\n");
		ret = -ENOMEM;
		goto err0;
	}

	dma_set_coherent_mask(&xhci->dev, dwc->dev->coherent_dma_mask);

<<<<<<< HEAD
=======
	xhci->dev.parent	= dwc->dev;
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	xhci->dev.dma_mask	= dwc->dev->dma_mask;
	xhci->dev.dma_parms	= dwc->dev->dma_parms;

	dwc->xhci = xhci;
<<<<<<< HEAD
	pdata.vendor = ((dwc->revision & DWC3_GSNPSID_MASK) >>
			__ffs(DWC3_GSNPSID_MASK) & DWC3_GSNPSREV_MASK);
	pdata.revision = dwc->revision & DWC3_GSNPSREV_MASK;

	ret = platform_device_add_data(xhci, (const void *) &pdata,
			sizeof(struct xhci_plat_data));
	if (ret) {
		dev_err(dwc->dev, "couldn't add pdata to xHCI device\n");
		goto err1;
	}

	ret = platform_device_add_resources(xhci, dwc->xhci_resources,
						DWC3_XHCI_RESOURCES_NUM);
=======

	/* setup resources */
	generic_resources[0].start = dwc->irq;

	generic_resources[1].start = dwc->res->start;
	generic_resources[1].end = dwc->res->start + 0x7fff;

	ret = platform_device_add_resources(xhci, generic_resources,
			ARRAY_SIZE(generic_resources));
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	if (ret) {
		dev_err(dwc->dev, "couldn't add resources to xHCI device\n");
		goto err1;
	}

<<<<<<< HEAD
	/* Add XHCI device if !OTG, otherwise OTG takes care of this */
	if (!dwc->dotg) {
		xhci->dev.parent = dwc->dev;
		ret = platform_device_add(xhci);
		if (ret) {
			dev_err(dwc->dev, "failed to register xHCI device\n");
			goto err1;
		}
=======
	ret = platform_device_add(xhci);
	if (ret) {
		dev_err(dwc->dev, "failed to register xHCI device\n");
		goto err1;
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	}

	return 0;

err1:
	platform_device_put(xhci);

err0:
	return ret;
}

void dwc3_host_exit(struct dwc3 *dwc)
{
<<<<<<< HEAD
	if (!dwc->dotg)
		platform_device_unregister(dwc->xhci);
=======
	platform_device_unregister(dwc->xhci);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
}
