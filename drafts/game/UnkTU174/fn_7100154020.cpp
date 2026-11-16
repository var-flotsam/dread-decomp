// Unit 174 (no reliable name): takes a pooled buffer object from the slot table, fills in its size and
// format, allocates its memory from one of two heaps (chosen by flag bit 0) and registers it with the
// device. Names and layouts are placeholders.
#include "game/UnkTU174.h"
#include "types.h"

struct UnkHeap_71001541a0;

struct UnkGfxHeap {
    u8 pool[0x100];
    u64 base;  // 0x100
    u8 pad_0x108[0x18];
    void* space;  // 0x120
};

struct UnkPooledBuffer_l154020 {
    void* field_0x0;
    void* block;  // 0x8
    u32 size;     // 0x10
    u32 field_0x14;
    u32 field_0x18;
    u32 flags;  // 0x1c
    u32 slot;   // 0x20
    u32 count;  // 0x24
    u64 field_0x28;
    u32 field_0x30;
    u8 field_0x34;
    u8 pad_0x35[3];
    u8 field_0x38[8];
};

extern const u32 g_unk_7101647bac[] __attribute__((visibility("hidden")));
extern const u32 g_unk_7101647e48[] __attribute__((visibility("hidden")));  // element size per format

extern UnkHeap_71001541a0* g_unk_716e29ab50;
extern UnkHeap_71001541a0* g_unk_716e29ab58;
extern void (*g_unk_716e29b038)(void* builder);
extern void (*g_unk_716e29b040)(void* builder, void* pool, u64 offset, u32 size);
extern void (*g_unk_716e29b068)(void* view, void* builder);
extern u64 (*g_unk_716e29b088)(void*);

void* fn_7100153dd0();
void* fn_7100153a10(UnkGfxHeap* heap, void* old, u64 size, u64 align);

// Bytes per element of a format (0 to 2), else 0.
inline u32 ElemSize(s32 format) {
    if ((u32)format <= 2)
        return g_unk_7101647e48[format];
    return 0;
}

// The heap a buffer's memory comes from: flag bit 0 picks the second one.
inline UnkHeap_71001541a0*& HeapFor(const UnkPooledBuffer_l154020* obj) {
    if (obj->flags & 1)
        return g_unk_716e29ab58;
    return g_unk_716e29ab50;
}

void fn_7100154020(UnkObj_71001541a0** out, u32 size, const u32* count, void* data, u32 a, u32 b) {
    *out = nullptr;
    u32 live = g_unk_716e29ab40;
    if (live >= 0x4000)
        return;
    auto* obj = reinterpret_cast<UnkPooledBuffer_l154020*>(g_unk_716e27ab38[live]);
    obj->field_0x0 = nullptr;
    obj->size = size;
    s32 kind = a - 2;
    obj->field_0x14 = (u32)kind <= 3 ? g_unk_7101647bac[kind] : 4;
    obj->field_0x18 = b == 3 ? 2 : (b != 1);
    obj->flags = *count;
    obj->slot = live;
    u32 elem = ElemSize(b - 1);
    obj->field_0x28 = 0;
    obj->field_0x30 = 0;
    obj->field_0x34 = 0;
    obj->count = size / elem;
    g_unk_716e29ab40 = live + 1;
    *out = reinterpret_cast<UnkObj_71001541a0*>(obj);

    void* first = fn_7100153dd0();
    g_unk_716e29b038(first);
    UnkHeap_71001541a0*& heap = HeapFor(obj);
    obj->block = fn_7100153a10(reinterpret_cast<UnkGfxHeap*>(heap), data, obj->size, 8);
    void* builder = fn_7100153dd0();
    UnkGfxHeap* gfx = reinterpret_cast<UnkGfxHeap*>(heap);
    g_unk_716e29b040(builder, gfx, (u64)obj->block - gfx->base, obj->size);
    void* last = fn_7100153dd0();
    g_unk_716e29b068(obj->field_0x38, last);
    obj->field_0x28 = g_unk_716e29b088(obj->field_0x38);
}
