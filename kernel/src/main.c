#include "fdt.h"
#include "initrd.h"
#include "lib/int.h"
#include "lib/mem.h"
#include "lib/uart.h"
#include "shell.h"
#include "utils.h"

void memncpy(void *src, void *dest, size_t n) {
  char *c_src = (char *)src;
  char *c_dest = (char *)dest;

  for (size_t i = 0; i < n; i++) {
    if (c_src[i] == '\0') return;
    c_dest[i] = c_src[i];
  }
}

int get_cur_el() {
  size_t cur_el;
  asm volatile("mrs %0, CurrentEL" : "=r"(cur_el));
  return (cur_el >> 2) & 0b11;
}

void kernel_main(char *dtb_ptr) {
  /* set up serial console */
  uart_init();
  // uart_async(1);

  uart_enable_interrupt();

  uart_printf("current el: %d\n", get_cur_el());

  if (fdt_traversal(dtb_ptr, initrd_addr) < 0) {
    prog_hang();
  }

  while (1) {
    asm volatile("nop");
  }

  /* say hello (test malloc) */
  char *hello_str = malloc(64);
  memcpy(hello_str, "Hello World from boot", 22);

  uart_println(hello_str);

  /* register commands */
  shell_t s = {};

  register_cmds(&s);

  shell_loop(&s);
}
