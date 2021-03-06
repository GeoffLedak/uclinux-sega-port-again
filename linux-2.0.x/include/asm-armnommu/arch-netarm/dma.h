#ifndef __ASM_ARCH_DMA_H
#define __ASM_ARCH_DMA_H

/*
 * This is the maximum DMA address that can be DMAd to (since there
 * are only 29 bits in a DMA buffer descriptor source buffer
 * pointer). 
 */
#define MAX_DMA_ADDRESS		0x1fffffff

/*
 * DMA modes - we have two, IN and OUT
 */
typedef enum {
	DMA_MODE_READ,
	DMA_MODE_WRITE
} dmamode_t;

/*
 * There are 10 DMA channels in the DMA controller module.  Normally,
 * this would be channels 0-9.  But on the NET+ARM, there are channels
 * 1-10.  So, we define MAX_DMA_CHANNELS as 11.  This will allow code
 * like "if (channel < MAX_DMA_CHANNELS)" to work properly.
 */
#define MAX_DMA_CHANNELS	11

#endif /* _ASM_ARCH_DMA_H */

