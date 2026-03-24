#include <stdint.h>
#include "resource_table_empty.h"
#include "pru_shared.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

/*
 * PRU shared RAM local address.
 * ARM side will map the same shared RAM through /dev/mem.
 */
#define PRU_SHARED_RAM      0x00010000

/* Default direct PRU output bit for your working test pin */
#define DEFAULT_OUT_MASK    (1u << 15)

/* Safe defaults */
#define DEFAULT_HIGH_COUNT  10000u
#define DEFAULT_LOW_COUNT   10000u

static void init_defaults(volatile pru_shared_t *ctrl)
{
    ctrl->magic      = PRU_SHARED_MAGIC;
    ctrl->enable     = 0u;
    ctrl->mode       = PRU_MODE_PWM;
    ctrl->out_mask   = DEFAULT_OUT_MASK;
    ctrl->high_count = DEFAULT_HIGH_COUNT;
    ctrl->low_count  = DEFAULT_LOW_COUNT;
    ctrl->reserved0  = 0u;
    ctrl->reserved1  = 0u;
}

void main(void)
{
    volatile pru_shared_t *ctrl = (volatile pru_shared_t *)PRU_SHARED_RAM;

    if (ctrl->magic != PRU_SHARED_MAGIC) {
        init_defaults(ctrl);
    }

    while (1) {
        uint32_t enable = ctrl->enable;
        uint32_t mode   = ctrl->mode;
        uint32_t mask   = ctrl->out_mask;
        uint32_t highc  = ctrl->high_count;
        uint32_t lowc   = ctrl->low_count;

        if (mask == 0u) {
            mask = DEFAULT_OUT_MASK;
        }
        if (highc == 0u) {
            highc = 1u;
        }
        if (lowc == 0u) {
            lowc = 1u;
        }

        if (!enable || mode == PRU_MODE_OFF) {
            __R30 &= ~mask;
            __delay_cycles(1000);
            continue;
        }

        if (mode == PRU_MODE_SQUARE) {
            __R30 ^= mask;
            __delay_cycles(highc);
            continue;
        }

        /* PWM mode */
        __R30 |= mask;
        __delay_cycles(highc);

        __R30 &= ~mask;
        __delay_cycles(lowc);
    }
}