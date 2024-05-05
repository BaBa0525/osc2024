#include "exception/handler.h"

#include "lib/int.h"
#include "lib/uart.h"

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

  return;
}
