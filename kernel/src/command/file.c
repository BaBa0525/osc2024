#include "command/file.h"

#include "cpio.h"
#include "lib/mem.h"
#include "lib/string.h"
#include "lib/uart.h"

void list_files() {
  for (file_iter_t it = cpio_list(); it.end != 1; file_iter_next(&it)) {
    uart_println(it.entry.filename);
  }
}

void cat_file(char *args, size_t n) {
  char filename[256];
  strtok(args, filename, n, ' ');

  for (file_iter_t it = cpio_list(); it.end != 1; file_iter_next(&it)) {
    if (strncmp(it.entry.filename, filename, n) != 0) {
      continue;
    }
    uart_print(it.entry.content);
    return;
  }

  uart_printf("file not found: '%s'\n", filename);
}

void execute_program(char *args, size_t n) {
  char filename[256];
  strtok(args, filename, n, ' ');

  char content[4096];
  int res = get_file_content(filename, content, 4096);

  if (res == 0) {
    uart_printf("file not found: '%s'\n", filename);
    return;
  }

  if (res == -1) {
    uart_printf("not support executable greater than 4096 bytes.\n");
    return;
  }

  char sp[4096];

  size_t spsr_el1 = 0x3c0;
  asm volatile("msr spsr_el1, %0" : : "r"(spsr_el1));
  asm volatile("msr elr_el1, %0" : : "r"(content));
  asm volatile("msr sp_el0, %0" : : "r"(sp));
  asm volatile("eret");
}
