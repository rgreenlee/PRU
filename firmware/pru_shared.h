#ifndef PRU_SHARED_H
#define PRU_SHARED_H

#include <stdint.h>

typedef struct {
    volatile uint32_t enable;
    volatile uint32_t half_period_cycles;
    volatile uint32_t gpio_mask;
    volatile uint32_t reserved;
} pru_shared_t;

#endif