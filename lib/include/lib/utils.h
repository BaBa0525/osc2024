#ifndef _LIB_UTILS_H
#define _LIB_UTILS_H

#include "int.h"

typedef volatile unsigned int vaddr_t;

u32_t mem_get32(vaddr_t *addr);

void mem_set32(vaddr_t *addr, u32_t val);

void wait_cycles(int c);

#endif  // _LIB_UTILS_H
