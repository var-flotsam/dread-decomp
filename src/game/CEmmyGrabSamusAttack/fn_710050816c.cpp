// Maps the name hash held in the object (at +0x0) to the index of one of 20 known names: "water" is
// 1, "speedbooster" 2, "speedbooster45up" 3, "left" 4, "right" 5, "patrol" 6, "search" 7 and so on up
// to "presetc" 19. Any other name gives 0.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkNameHolder_50816c {
    u64 hash;
};

u32 fn_710050816c(UnkNameHolder_50816c* self) {
    if (StringCrc64("water", 5) == self->hash)
        return 1;
    if (StringCrc64("speedbooster", 12) == self->hash)
        return 2;
    if (StringCrc64("speedbooster45up", 16) == self->hash)
        return 3;
    if (StringCrc64("left", 4) == self->hash)
        return 4;
    if (StringCrc64("right", 5) == self->hash)
        return 5;
    if (StringCrc64("patrol", 6) == self->hash)
        return 6;
    if (StringCrc64("search", 6) == self->hash)
        return 7;
    if (StringCrc64("chase", 5) == self->hash)
        return 8;
    if (StringCrc64("chase2", 6) == self->hash)
        return 9;
    if (StringCrc64("chasereachable", 14) == self->hash)
        return 10;
    if (StringCrc64("combat", 6) == self->hash)
        return 11;
    if (StringCrc64("flee", 4) == self->hash)
        return 12;
    if (StringCrc64("brokenshield", 12) == self->hash)
        return 13;
    if (StringCrc64("grab2", 5) == self->hash)
        return 14;
    if (StringCrc64("grabwater", 9) == self->hash)
        return 15;
    if (StringCrc64("protoemmytuto", 13) == self->hash)
        return 16;
    if (StringCrc64("preseta", 7) == self->hash)
        return 17;
    if (StringCrc64("presetb", 7) == self->hash)
        return 18;
    return StringCrc64("presetc", 7) == self->hash ? 19 : 0;
}
