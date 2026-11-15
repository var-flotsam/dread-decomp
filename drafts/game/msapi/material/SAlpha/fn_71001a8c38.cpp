// Unit msapi::material::SAlpha: shifts the pointer members of a block by the distance between two bases.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSAlphaPointers {
    u8* head[3];
    u8 pad_0x18[0x118];
    u8* tail[6];
};

void fn_71001a8c38(UnkSAlphaPointers* self, u8* oldBase, u64 unused, u8* newBase) {
    for (u8*& ptr : self->head)
        if (oldBase != newBase && ptr)
            ptr = (newBase - oldBase) + ptr;
    for (u8*& ptr : self->tail)
        if (oldBase != newBase && ptr)
            ptr = (newBase - oldBase) + ptr;
}
