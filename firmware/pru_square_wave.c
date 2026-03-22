#include <stdint.h>

/*
 * BBB PRU0 square-wave output on R30 bit 0
 *
 * On BeagleBone Black, PRU0 R30 bit 0 maps to P9_31
 * when pinmux is configured for PRU output.
 */

volatile register uint32_t __R30;
volatile register uint32_t __R31;

#define OUTPUT_BIT   (1u << 0)   // PRU0 R30 bit 0 -> P9_31
#define DELAY_CYCLES 10000000u   // adjust for visible blink / test rate

static void delay_cycles(volatile uint32_t count)
{
    while (count--) {
        __asm__("NOP");
    }
}

void main(void)
{
    while (1) {
        __R30 ^= OUTPUT_BIT;
        delay_cycles(DELAY_CYCLES);
    }
}