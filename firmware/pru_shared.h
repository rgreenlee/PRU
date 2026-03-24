#ifndef PRU_SHARED_H
#define PRU_SHARED_H

#include <stdint.h>

#define PRU_SHARED_MAGIC   0x50525531u   /* "PRU1" */

#define PRU_MODE_OFF       0u
#define PRU_MODE_PWM       1u
#define PRU_MODE_SQUARE    2u

typedef struct {
    volatile uint32_t magic;
    volatile uint32_t enable;
    volatile uint32_t mode;
    volatile uint32_t out_mask;
    volatile uint32_t high_count;
    volatile uint32_t low_count;
    volatile uint32_t reserved0;
    volatile uint32_t reserved1;
} pru_shared_t;

#endif