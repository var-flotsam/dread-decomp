#pragma once

#include "game/UnkTU1141.h"

struct alignas(8) UnkAnimChannel_ovb0125 {
    u8 data[0x70];
    UnkAnimChannel_ovb0125() { fn_71007142e8(reinterpret_cast<UnkStruct_71007142e8*>(this)); }
    UnkWeighted_7100714808* Weighted() { return reinterpret_cast<UnkWeighted_7100714808*>(this); }
};
