#ifndef RESOURCE_TABLE_EMPTY_H
#define RESOURCE_TABLE_EMPTY_H

#include <stdint.h>

struct my_resource_table {
    uint32_t ver;
    uint32_t num;
    uint32_t reserved[2];
};

#pragma DATA_SECTION(resourceTable, ".resource_table")
#pragma RETAIN(resourceTable)
const struct my_resource_table resourceTable = {
    1,
    0,
    {0, 0}
};

#endif