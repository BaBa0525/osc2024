#define MMIO_BASE 0x3F000000
#define AUX_MU_IO_REG ((char *)(MMIO_BASE + 0x00215040))
#define AUX_MU_LSR_REG ((char *)(MMIO_BASE + 0x00215054))

int main();

void _start() { main(); }

void set32(char *addr, unsigned int val) {
  asm volatile("str %w[v], [%[a]]" ::[a] "r"(addr), [v] "r"(val));
}

unsigned int get32(char *addr) {
  unsigned int val;
  asm volatile("ldr %w[v], [%[a]]" : [v] "=r"(val) : [a] "r"(addr));
  return val;
}

void uart_putc_raw(char c) {
  while ((get32(AUX_MU_LSR_REG) & (1 << 5)) == 0) {
    asm volatile("nop");
  }

  set32(AUX_MU_IO_REG, c);
}

void uart_putc(char c) {
  if (c == '\n') {
    uart_putc_raw('\r');
  }
  uart_putc_raw(c);
}

void uart_puts(char *s) {
  while (*s != '\0') {
    uart_putc(*s++);
  }
}

int main() {
  uart_puts("Hello from EL0.\n");

  for (;;) {
  }

  return 0;
}
