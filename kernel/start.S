.section ".text.boot"

.global _start
.global prog_hang

_start:
	// move dbt ptr to an unused register
	mov x19, x0

	bl from_el2_to_el1

	bl set_vbar_el1

	/* enable core timer in el0*/
	// bl core_timer_enable

	// read cpu id, stop slave cores
	// the last 8 bits indicate the core number in cortex-a53
	mrs x1, mpidr_el1
	and x1, x1, #3
	cbz x1, setup

	// if core number > 0, let the core hangs
prog_hang:
	wfe
	b prog_hang

from_el2_to_el1:
	// enable aarch64 in el1
	mov x0, (1 << 31)
	msr hcr_el2, x0

	// EL1h(SP_SEL = 1) and disable interrupt
	/*
	 * ref: 
	 * https://developer.arm.com/documentation/ddi0595/2021-03/AArch64-Registers/SPSR-EL2--Saved-Program-Status-Register--EL2-
	 * https://developer.arm.com/documentation/100933/0100/Processor-state-in-exception-handling
	 * 3
	 * |_1 -- Debug exception mask.
	 * |_1 -- A: SError.
	 * 
	 * 4
	 * |_0 -- I: IRQ interrupt mask. We want to enable interrupt in EL1, so set it to 0.
	 * |_1 -- F: FIQ interrupt mask.
	 * |_0 -- Bit: Reserved.
	 * |_0 -- Execution state.
	 *
	 * 5
	 * |_01 -- EL.
	 * |_0  -- Reserved.
	 * |_1  -- SP: current EL(SP_EL1)
	 */
	mov x0, 0x345
	msr spsr_el2, x0

	msr elr_el2, lr
	eret

setup:
	// set top of stack just before our code (stack grows to a lower address)
	ldr x1, =_start
	mov sp, x1

	ldr x1, =__bss_start

	// w- registers are the 32-bit version of x- register
	ldr w2, =__bss_size

clear_bss:
	// clear bss
	cbz  w2, jump_main
	str  xzr, [x1], #8
	sub  w2, w2, #1
	cbnz w2, clear_bss

jump_main:
	// load dbt ptr to the first parameter
	mov x0, x19

	// jump to C code, should not return
	bl kernel_main

	// for failsafe, halt this core too
	b prog_hang

