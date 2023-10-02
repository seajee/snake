#include "util.h"

#include <stdlib.h>

int32_t rand_range(int32_t min, int32_t max)
{
    return (rand() % (max - min)) + min;
}
