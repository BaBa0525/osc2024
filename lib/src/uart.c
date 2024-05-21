#include "uart.h"

#include "gpio.h"
#include "io.h"
#include "utils.h"

/* Auxilary mini UART registers */
#define AUX_ENABLE ((vaddr_t *)(MMIO_BASE + 0x00215004))
#define AUX_MU_IO ((vaddr_t *)(MMIO_BASE + 0x00215040))
#define AUX_MU_IER ((vaddr_t *)(MMIO_BASE + 0x00215044))
#define AUX_MU_IIR ((vaddr_t *)(MMIO_BASE + 0x00215048))
#define AUX_MU_LCR ((vaddr_t *)(MMIO_BASE + 0x0021504C))
#define AUX_MU_MCR ((vaddr_t *)(MMIO_BASE + 0x00215050))
#define AUX_MU_LSR ((vaddr_t *)(MMIO_BASE + 0x00215054))
#define AUX_MU_MSR ((vaddr_t *)(MMIO_BASE + 0x00215058))
#define AUX_MU_SCRATCH ((vaddr_t *)(MMIO_BASE + 0x0021505C))
#define AUX_MU_CNTL ((vaddr_t *)(MMIO_BASE + 0x00215060))
#define AUX_MU_STAT ((vaddr_t *)(MMIO_BASE + 0x00215064))
#define AUX_MU_BAUD ((vaddr_t *)(MMIO_BASE + 0x00215068))

#define RX_INTERRUPT 0x01
#define TX_INTERRUPT 0x02

#define ENABLE_IRQS_1 ((vaddr_t *)(MMIO_BASE + 0xB210))
#define AUX_INT (1 << 29)

void uart_async(int use) {
  /* enable uart interrupt */
  if (use) {
    u32_t ier = mem_get32(AUX_MU_IER);

    ier |= RX_INTERRUPT;
    mem_set32(AUX_MU_IER, ier);

    /* enable second level interrupt controller */
    u32_t enable_irqs_1 = mem_get32(ENABLE_IRQS_1);
    mem_set32(ENABLE_IRQS_1, enable_irqs_1 | AUX_INT);
  }
};

void uart_init() {
  /* function selector */
  register unsigned int selector;

  /* map UART1 to GPIO pins */
  selector = mem_get32(GPFSEL1);

  /*
   * clean gpio14, gpio15
   * SEL0 -> 1-10, SEL1 -> 11-20
   */
  selector &= ~((7 << 12) | (7 << 15));

  /* alt5 because we use mini uart */
  selector |= (2 << 12) | (2 << 15);

  mem_set32(GPFSEL1, selector);

  /* enable pins 14 and 15 */
  mem_set32(GPPUD, 0);
  wait_cycles(150);

  mem_set32(GPPUDCLK0, 1 << 14 | 1 << 15);
  wait_cycles(150);

  /* flush GPIO setup */
  mem_set32(GPPUDCLK0, 0);

  /* initialize UART */
  /* enable mini uart */
  mem_set32(AUX_ENABLE, mem_get32(AUX_ENABLE) | 1);

  /* disable transmitter and receiver during configuration */
  mem_set32(AUX_MU_CNTL, 0);

  /* disable interrupt */
  mem_set32(AUX_MU_IER, 0);

  /* set the data size to 8 bit */
  mem_set32(AUX_MU_LCR, 3);

  /* do not nedd auto flow control */
  mem_set32(AUX_MU_MCR, 0);

  /* set baud rate to 115200 */
  mem_set32(AUX_MU_BAUD, 270);

  mem_set32(AUX_MU_IIR, 6);
  mem_set32(AUX_MU_CNTL, 3); /* enable Tx, Rx */
}

void uart_write(unsigned int c) {
  // wait until we can send
  do {
    asm volatile("nop");
  } while (!(*AUX_MU_LSR & 0x20));
  // write char to the buffer
  *AUX_MU_IO = c;
}

char uart_read_raw() {
  char r;
  // wait until something is in the buffer
  do {
    asm volatile("nop");
  } while (!(*AUX_MU_LSR & 0x01));

  r = (char)(*AUX_MU_IO);

  return r;
}

char uart_read() {
  char r = uart_read_raw();
  // convert carriage return to newline
  return r == '\r' ? '\n' : r;
}

void uart_printf(char *fmt, ...) {
  __builtin_va_list args;
  __builtin_va_start(args, fmt);

  char str_buf[1024];
  vsprintk(str_buf, fmt, args);

  int index = 0;
  while (str_buf[index] && index < 1024) {
    uart_write(str_buf[index++]);
  }
}

void uart_print(char *s) {
  while (*s) {
    if (*s == '\n') {
      // convert newline to carriage return + newline
      uart_write('\r');
    }
    uart_write(*s++);
  }
}

void uart_println(char *s) {
  uart_print(s);

  // newline in the end
  uart_print("\n");
}

void uart_hex(size_t d) {
  unsigned int n;
  int c;

  for (c = 60; c >= 0; c -= 4) {
    // get highest tetrad
    n = (d >> c) & 0xF;

    // 0-9 => '0'-'9', 10-15 => 'A'-'F'
    n += n > 9 ? 0x37 : 0x30;
    uart_write(n);
  }
}
