// In CWorldGraph's unit: the index of an element in the array of 0x80-byte elements at +0x30, found
// by address; -1 when it isn't one of them. Names and the layout are placeholders.
#include "types.h"

struct UnkWorldGraphNode {
    u8 unk_0x0[0x80];
};

struct UnkWorldGraphNodes {
    u8 pad_0x0[0x30];
    UnkWorldGraphNode* data;
    u32 count;
};

s32 fn_7100acca84(const UnkWorldGraphNodes* self, const UnkWorldGraphNode* node) {
    for (u64 i = 0; i < self->count; ++i) {
        if (&self->data[i] == node)
            return (s32)i;
    }
    return -1;
}
