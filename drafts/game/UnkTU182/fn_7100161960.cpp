// Unit 182: takes the next free object from the fixed pool (0x4000 slots), resets it, allocates its
// buffer from one of two heaps by its flags and registers the buffer and its block with the device.
// Names and layouts are placeholders.
#include "game/UnkTU182.h"
#include "types.h"

// A zero whose constructor isn't constexpr, so the statics are guarded.
struct UnkStaticZero_ovb0224 {
    u32 value;
    UnkStaticZero_ovb0224() : value(0) {}
};

inline const UnkStaticZero_ovb0224& SecondZero_ovb0224() {
    static UnkStaticZero_ovb0224 s_value asm("g_unk_716e34ae40");
    return s_value;
}

struct UnkGfxHeap {
    u8 pool[0x100];
    u64 base;  // 0x100
    u8 pad_0x108[0x18];
    void* space;  // 0x120
};

struct UnkGfxPoolChild_l161960;

// The pooled object (UnkObj_7100161b90 in the header's declarations).
struct UnkGfxPoolObject_l161960 {
    void* data;
    u64 field_0x8;
    void* buffer;
    u32 field_0x18;
    u32 size;
    u32 index;
    u32 flags;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    bool field_0x34;
    UnkGfxPoolChild_l161960* children[16];
    u8 pad_0xb8[0xc0];
    u8 block_0x178[1];
};

extern UnkGfxPoolObject_l161960* g_unk_716e2aadf8[];  // the pool's objects, live ones first
extern u32 g_unk_716e2cadf8;                          // how many are live
extern UnkGfxHeap* g_unk_716e34ae20;                  // the buffer heap for flagged objects
extern UnkGfxHeap* g_unk_716e34ae28;                  // the buffer heap for the others
extern void (*g_unk_716e29b038)(void* builder);
extern void (*g_unk_716e29b040)(void* builder, UnkGfxHeap* heap, u64 offset, u32 size);
extern void (*g_unk_716e29b068)(void* block, void* builder);

void* fn_7100153a10(UnkGfxHeap* heap, void* old, u64 size, u64 align);
void* fn_7100153dd0();

// Allocates the object's buffer from a heap and registers it, and the object's block, with the device.
inline void AllocateBuffer(UnkGfxPoolObject_l161960* obj, UnkGfxHeap*& heap) {
    obj->buffer = fn_7100153a10(heap, obj->data, obj->size, 8);
    void* builder = fn_7100153dd0();
    u64 offset = reinterpret_cast<u64>(obj->buffer) - heap->base;
    g_unk_716e29b040(builder, heap, offset, obj->size);
    void* block = obj->block_0x178;
    builder = fn_7100153dd0();
    g_unk_716e29b068(block, builder);
}

void fn_7100161960(UnkObj_7100161b90** out, u32 size, void* data, const u32* count) {
    *out = nullptr;
    u32 index = g_unk_716e2cadf8;
    if (index < 0x4000) {
        UnkGfxPoolObject_l161960* obj = g_unk_716e2aadf8[index];
        obj->field_0x18 = 0;
        obj->size = size;
        obj->data = nullptr;
        obj->field_0x8 = 0;
        obj->index = index;
        obj->flags = *count;
        obj->field_0x28 = SecondZero_ovb0224().value;
        obj->field_0x2c = 0;
        obj->field_0x30 = 0;
        obj->field_0x34 = false;
        for (s32 i = 0; i < 16; ++i)
            obj->children[i] = nullptr;
        obj->field_0x18 = 0;
        obj->field_0x28 = SecondZero_ovb0224().value;
        obj->field_0x2c = 0;
        obj->field_0x30 = 0;
        g_unk_716e2cadf8++;
        *out = reinterpret_cast<UnkObj_7100161b90*>(obj);
        if (data)
            obj->data = data;

        void* builder = fn_7100153dd0();
        g_unk_716e29b038(builder);
        if (obj->flags & 1)
            AllocateBuffer(obj, g_unk_716e34ae20);
        else
            AllocateBuffer(obj, g_unk_716e34ae28);
    }
}
