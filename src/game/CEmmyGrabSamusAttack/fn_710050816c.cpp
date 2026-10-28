// Maps the name hash held in the object (at +0x0) to the index of one of 20 known names: "water" is
// 1, "speedbooster" 2, "speedbooster45up" 3, "left" 4, "right" 5, "patrol" 6, "search" 7 and so on up
// to "presetc" 19. Any other name gives 0.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkNameHolder {
    u64 hash;
};

u32 fn_710050816c(UnkNameHolder* self) {
    if (fn_7100001570("water", 5) == self->hash)
        return 1;
    if (fn_7100001570("speedbooster", 12) == self->hash)
        return 2;
    if (fn_7100001570("speedbooster45up", 16) == self->hash)
        return 3;
    if (fn_7100001570("left", 4) == self->hash)
        return 4;
    if (fn_7100001570("right", 5) == self->hash)
        return 5;
    if (fn_7100001570("patrol", 6) == self->hash)
        return 6;
    if (fn_7100001570("search", 6) == self->hash)
        return 7;
    if (fn_7100001570("chase", 5) == self->hash)
        return 8;
    if (fn_7100001570("chase2", 6) == self->hash)
        return 9;
    if (fn_7100001570("chasereachable", 14) == self->hash)
        return 10;
    if (fn_7100001570("combat", 6) == self->hash)
        return 11;
    if (fn_7100001570("flee", 4) == self->hash)
        return 12;
    if (fn_7100001570("brokenshield", 12) == self->hash)
        return 13;
    if (fn_7100001570("grab2", 5) == self->hash)
        return 14;
    if (fn_7100001570("grabwater", 9) == self->hash)
        return 15;
    if (fn_7100001570("protoemmytuto", 13) == self->hash)
        return 16;
    if (fn_7100001570("preseta", 7) == self->hash)
        return 17;
    if (fn_7100001570("presetb", 7) == self->hash)
        return 18;
    return fn_7100001570("presetc", 7) == self->hash ? 19 : 0;
}
