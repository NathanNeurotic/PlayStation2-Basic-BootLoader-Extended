#pragma once

#include "ps2sdk_shim.h"

static inline int btoi(unsigned char c)
{
    return ((c >> 4) * 10) + (c & 0x0F);
}
