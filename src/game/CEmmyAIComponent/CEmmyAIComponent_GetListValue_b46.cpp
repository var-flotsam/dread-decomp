// CEmmyAIComponent's unit (tools/tu_map.py unit 811): two getters that, when the list at +0x29560
// (or +0x29610; count at +8) is not empty, write its value (fn_7100030dec) to `out` and return
// true. Names and layouts are placeholders.
#include "game/UnkTU39.h"
#include "types.h"

struct UnkFloatList_b46a1 {
    void* data;
    u32 count;
};

struct UnkEmmyAILists_b46a1 {
    u8 pad_0x0[0x29560];
    UnkFloatList_b46a1 list_0x29560;
    u8 pad_0x29570[0x29610 - 0x29570];
    UnkFloatList_b46a1 list_0x29610;
};

static inline __attribute__((always_inline)) bool GetValue(UnkFloatList_b46a1* list, f32* out) {
    if (list->count == 0)
        return false;
    *out = fn_7100030dec(list);
    return true;
}

bool fn_710055b574(UnkEmmyAILists_b46a1* self, f32* out) { return GetValue(&self->list_0x29560, out); }
bool fn_710055b5c8(UnkEmmyAILists_b46a1* self, f32* out) { return GetValue(&self->list_0x29610, out); }
