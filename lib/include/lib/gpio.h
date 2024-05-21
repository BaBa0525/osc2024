#ifndef _GPIO_H
#define _GPIO_H

#include "utils.h"

#define MMIO_BASE 0x3F000000

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

#endif  // _GPIO_H
