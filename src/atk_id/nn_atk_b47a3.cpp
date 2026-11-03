// nn::atk unit (tools/tu_map.py): small accessors of the sound library's objects (a clamped volume setter,
// a kind mask test, list counts and walks, a buffer initialiser, two size computations). Names and
// layouts are placeholders.
#include "types.h"

struct UnkAtkLink_b47a3 {
    UnkAtkLink_b47a3* prev;
    UnkAtkLink_b47a3* next;
};

struct UnkAtkVolumeHolder_b47a3 {
    u8 unk_0x0[0x1e8];
    u16 flags;
    u8 unk_0x1ea[2];
    f32 volume;
};

static inline f32 ClampLevel_b47a3(f32 v) { return v < 0.0f ? 0.0f : v; }

void fn_710122d748(UnkAtkVolumeHolder_b47a3* self, f32 level) {
    f32 volume = ClampLevel_b47a3(level);
    if (volume != self->volume) {
        self->volume = volume;
        self->flags |= 0x40;
    }
}

extern u32 g_unk_7101cb79c8 __attribute__((visibility("hidden")));

bool fn_71012136d0(s32 kind) {
    u32 mask;
    switch (kind) {
    case 0: mask = 31; break;
    case 1: mask = 1; break;
    case 2: mask = 2; break;
    case 3: mask = 4; break;
    case 4: mask = 8; break;
    case 5: mask = 16; break;
    default: mask = 0; break;
    }
    return (mask & ~g_unk_7101cb79c8) == 0;
}

struct UnkAtkListHolder_b47a3 {
    u8 unk_0x0[0x890];
    UnkAtkLink_b47a3 head;
};

u32 fn_710121532c(UnkAtkListHolder_b47a3* self) {
    u32 n = 0;
    for (UnkAtkLink_b47a3* it = self->head.next; it != &self->head; it = it->next)
        ++n;
    return n;
}

u64 fn_710120f124(void* param);

u64 fn_710122ee14(void* self, s32 count, void* param) {
    u64 extra = fn_710120f124(param);
    u64 size = extra ? extra + 0x2b0 : 0x298;
    return size * count;
}

struct UnkAtkFlagged_b47a3 {
    bool active;
    u8 unk_0x1[0xf];
    u16 flags;
    u8 unk_0x12[0xb0 - 0x12];
    UnkAtkLink_b47a3 link;
};

struct UnkAtkFlaggedList_b47a3 {
    u8 unk_0x0[8];
    UnkAtkLink_b47a3 head;
};

void fn_710122f510(UnkAtkFlaggedList_b47a3* self, u32 mask) {
    UnkAtkLink_b47a3* it = self->head.next;
    while (&self->head != it) {
        UnkAtkLink_b47a3* next = it->next;
        UnkAtkFlagged_b47a3* e = reinterpret_cast<UnkAtkFlagged_b47a3*>(reinterpret_cast<char*>(it) - 0xb0);
        if (e->active)
            e->flags |= mask;
        it = next;
    }
}

struct UnkAtkBuffer_b47a3 {
    u8* data;
    s32 size;
};

bool fn_7101212d98(UnkAtkBuffer_b47a3* self, u8* data, void*, s32 size) {
    self->data = data;
    self->size = size;
    for (s32 i = 0; i < self->size; ++i)
        self->data[i] = 0;
    return true;
}

u64 fn_710122c75c(void* self, s32 count, void* param) {
    u64 extra = fn_710120f124(param);
    u64 size = extra ? (extra + 0x418 + 3) & ~(u64)3 : 0x400;
    return size * count + 0x70;
}
