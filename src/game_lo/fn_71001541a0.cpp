// Returns a pooled object to its pool: finalises it, frees its memory block from one of two
// heaps (chosen by flag bit 0), and swaps it with the last live entry of the pool's slot table,
// marking it unused (-1).
#include "types.h"

struct UnkHeap_71001541a0;
void fn_7100153a88(UnkHeap_71001541a0* heap, void* block);

struct UnkObj_71001541a0 {
    void* field_0x0;
    void* block;  // 0x8
    u8 unk_0x10[0xc];
    u32 flags;  // 0x1c
    u32 slot;   // 0x20
    u8 unk_0x24[0x14];
    u8 field_0x38;
};

extern u32 g_unk_716e29ab40;                    // live count
extern UnkObj_71001541a0* g_unk_716e27ab38[];   // slot table
extern UnkHeap_71001541a0* g_unk_716e29ab50;
extern UnkHeap_71001541a0* g_unk_716e29ab58;
extern void (*g_unk_716e29b078)(void* data);   // finaliser

// NON_MATCHING(minor): the finaliser pointer's GOT load is scheduled before the store that clears
// field 0x0 instead of after it.
void fn_71001541a0(UnkObj_71001541a0* obj) {
    if (!obj || g_unk_716e29ab40 == 0)
        return;
    u32 slot = obj->slot;
    if (slot >= 0x4000)
        return;
    obj->field_0x0 = nullptr;
    g_unk_716e29b078(&obj->field_0x38);
    fn_7100153a88((obj->flags & 1) == 0 ? g_unk_716e29ab50 : g_unk_716e29ab58, obj->block);
    obj->block = nullptr;
    u32 last = --g_unk_716e29ab40;
    UnkObj_71001541a0* other = g_unk_716e27ab38[last];
    g_unk_716e27ab38[slot] = other;
    other->slot = slot;
    g_unk_716e27ab38[last] = obj;
    obj->slot = -1;
}
