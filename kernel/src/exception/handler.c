#include "exception/handler.h"

#include "lib/int.h"
#include "lib/mmio.h"
#include "lib/uart.h"
#include "lib/utils.h"

void exception_handler_c() {
  uart_printf("Exception Handler!\n");

  u64_t spsr_el1;
  asm volatile("mrs %0, spsr_el1" : "=r"(spsr_el1));
  uart_printf("spsr_el1: %x\n", spsr_el1);

  u64_t elr_el1;
  asm volatile("mrs %0, elr_el1" : "=r"(elr_el1));
  uart_printf("elr_el1: %x\n", elr_el1);

  u64_t esr_el1;
  asm volatile("mrs %0, esr_el1" : "=r"(esr_el1));
  uart_printf("esr_el1: %x\n", esr_el1);

  short ec = (esr_el1 >> 26) & 0b111111;
  uart_printf("ec: %x\n", ec);

  for (;;) {
  }
}

#define AUX_MU_IO ((vaddr_t *)(MMIO_BASE + 0x00215040))

void irq_handler_c() {
  uart_printf("IRQ Handler!\n");

  char c = mem_get32(AUX_MU_IO);
  uart_write(c);

  return;

  u64_t cntpct_el0;
  asm volatile("mrs %0, cntpct_el0" : "=r"(cntpct_el0));

  u64_t cntfrq_el0;
  asm volatile("mrs %0, cntfrq_el0" : "=r"(cntfrq_el0));

  u64_t boot_time = cntpct_el0 / cntfrq_el0;
  uart_printf("boot time: %d seconds\n", boot_time);

  u64_t next_timeout = 2;
  asm volatile("msr cntp_tval_el0, %0" ::"r"(next_timeout));

  for (;;) {
  }
}
