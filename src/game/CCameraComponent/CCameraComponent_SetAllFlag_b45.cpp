// CCameraComponent's unit (tools/tu_map.py): sets the flag at +0x98 of every entry of the list at
// +0x80. The flag is written as a plain byte, so the list's array and count are read again after each
// store, as in the original. Names and layouts are placeholders.
#include "types.h"

struct UnkCameraEntry_b45a3;

struct UnkCameraList_b45a3 {
    u8 pad_0x0[0x80];
    UnkCameraEntry_b45a3** data;  // 0x80
    u8 pad_0x88[0x8];
    u32 count;  // 0x90
};

static inline void SetFlag(UnkCameraEntry_b45a3* entry, bool value) {
    *(reinterpret_cast<u8*>(entry) + 0x98) = value;
}

void fn_71010e380c(UnkCameraList_b45a3* self, bool value) {
    for (UnkCameraEntry_b45a3** it = self->data; it != self->data + self->count; ++it)
        SetFlag(*it, value);
}
