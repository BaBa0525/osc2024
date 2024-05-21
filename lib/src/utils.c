#include "utils.h"

inline u32_t mem_get32(vaddr_t *addr) { return *addr; }

inline void mem_set32(vaddr_t *addr, u32_t val) { *addr = val; }

void wait_cycles(int c) {
  while (c--) {
    asm volatile("nop");
  }
}
