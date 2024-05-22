#ifndef _MMIO_H
#define _MMIO_H

#include "utils.h"

#define MMIO_BASE 0x3F000000

/* gpio */

#define GPFSEL_BASE (MMIO_BASE + 0x00200000)

#define GPFSEL0 ((vaddr_t *)(GPFSEL_BASE))
#define GPFSEL1 ((vaddr_t *)(GPFSEL_BASE + 0x04))
#define GPFSEL2 ((vaddr_t *)(GPFSEL_BASE + 0x08))
#define GPFSEL3 ((vaddr_t *)(GPFSEL_BASE + 0x0C))
#define GPFSEL4 ((vaddr_t *)(GPFSEL_BASE + 0x10))
#define GPFSEL5 ((vaddr_t *)(GPFSEL_BASE + 0x14))

#define GPSET0 ((vaddr_t *)(GPFSEL_BASE + 0x1C))
#define GPSET1 ((vaddr_t *)(GPFSEL_BASE + 0x20))

#define GPCLR0 ((vaddr_t *)(GPFSEL_BASE + 0x28))

#define GPLEV0 ((vaddr_t *)(GPFSEL_BASE + 0x34))
#define GPLEV1 ((vaddr_t *)(GPFSEL_BASE + 0x38))

#define GPEDS0 ((vaddr_t *)(GPFSEL_BASE + 0x40))
#define GPEDS1 ((vaddr_t *)(GPFSEL_BASE + 0x44))

#define GPHEN0 ((vaddr_t *)(GPFSEL_BASE + 0x64))
#define GPHEN1 ((vaddr_t *)(GPFSEL_BASE + 0x68))

#define GPPUD ((vaddr_t *)(GPFSEL_BASE + 0x94))
#define GPPUDCLK0 ((vaddr_t *)(GPFSEL_BASE + 0x98))
#define GPPUDCLK1 ((vaddr_t *)(GPFSEL_BASE + 0x9C))

/*
 * Auxiliaries
 * raspi3b+ has three auxiliary peripherals: One mini UART and two SPI masters.
 * These three peripheral are grouped together as they share the same area in
 * the peripheral register map and they share a common interrupt. Also all three
 * are controlled by the auxiliary enable register.
 *
 * ref: https://cs140e.sergio.bz/docs/BCM2837-ARM-Peripherals.pdf
 * 2. Auxiliaries: UART1 & SPI1, SPI2
 */

/*
 * Auxiliary enables
 * size: 3 bits
 */
#define AUX_ENABLES ((vaddr_t *)(MMIO_BASE + 0x00215004))

/* Auxilary mini UART registers */

/*
 * Mini Uart I/O Data
 * size: 8 bits
 */
#define AUX_MU_IO_REG ((vaddr_t *)(MMIO_BASE + 0x00215040))

/*
 * Mini Uart Interrupt Enable
 * size: 8 bits
 */
#define AUX_MU_IER_REG ((vaddr_t *)(MMIO_BASE + 0x00215044))

#define AUX_MU_IIR_REG ((vaddr_t *)(MMIO_BASE + 0x00215048))
#define AUX_MU_LCR_REG ((vaddr_t *)(MMIO_BASE + 0x0021504C))
#define AUX_MU_MCR_REG ((vaddr_t *)(MMIO_BASE + 0x00215050))
#define AUX_MU_LSR_REG ((vaddr_t *)(MMIO_BASE + 0x00215054))
#define AUX_MU_MSR_REG ((vaddr_t *)(MMIO_BASE + 0x00215058))
#define AUX_MU_SCRATCH_REG ((vaddr_t *)(MMIO_BASE + 0x0021505C))
#define AUX_MU_CNTL_REG ((vaddr_t *)(MMIO_BASE + 0x00215060))
#define AUX_MU_STAT_REG ((vaddr_t *)(MMIO_BASE + 0x00215064))
#define AUX_MU_BAUD_REG ((vaddr_t *)(MMIO_BASE + 0x00215068))

#endif  // _MMIO_H
