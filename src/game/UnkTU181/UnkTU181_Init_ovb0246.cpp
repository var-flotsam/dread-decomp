// Unit 181 (no reliable name): the unit's static initialiser and the flush of changed uniform blocks. Names and layouts are placeholders.
#include <new>
#include "base/UnkAtExit.h"
#include "base/UnkPairArrays.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGraphicsState.h"
#include "game/UnkTU172.h"
#include "game/UnkTU180.h"
#include "game/UnkTU182.h"
#include "types.h"

using base::global::CStrId;

struct UnkObject_0x130;
struct UnkGfxHeap;
struct UnkPool;

void fn_71001537c0(UnkPool* pool);
void fn_71001537d4(UnkPool* pool);
UnkObject_0x130* fn_7100153a90(s32 index);
void fn_7100094798(void* vector);
void fn_710015e9a4(void* arrays);

extern UnkObject_0x130* g_unk_716e2a76a0;
extern base::UnkPairArrays g_unk_716e2a9708;

struct UnkEntry181_b0246 {
    u8 unk_0x0[8];
};

// The unit's statics, laid out as the compiler placed them: the header and a vector.
struct UnkUnit181Statics_b0246 {
    u32 header;
    CStrId binaries;
    CStrId sources;
    base::UnkVector<UnkEntry181_b0246> entries;  // 0x18
};
extern UnkUnit181Statics_b0246 g_unk_716e2aadb0 __attribute__((visibility("hidden")));

// One of the slots the graphics state binds (0x78 bytes), as it starts.
struct UnkSlotInit_b0246 {
    u64 ptr;          // 0x0
    u32 field_0x8;
    u8 pad_0xc[4];
    s32 state;        // 0x10
    s32 field_0x14;
    s32 field_0x18;
    s32 transition;   // 0x1c
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64;
    u64 field_0x68;
    u16 field_0x70;
    u8 pad_0x72[6];
};
static_assert(sizeof(UnkSlotInit_b0246) == 0x78, "UnkSlotInit_b0246 layout");

void fn_710015ef10() {
    UnkUnit181Statics_b0246& s = g_unk_716e2aadb0;
    s.header = 6;
    new (&s.binaries) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &s.binaries, g_unk_7101a19000);
    new (&s.sources) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &s.sources, g_unk_7101a19000);
    fn_71001537c0(&g_unk_716e2a7580);
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_71001537d4), &g_unk_716e2a7580, g_unk_7101a19000);
    g_unk_716e2a76a0 = fn_7100153a90(2);
    g_unk_716e2a76a8 = reinterpret_cast<UnkGfxHeap*>(fn_7100153a90(3));
    UnkObject_0x130* heap = fn_7100153a90(1);
    s.entries.data = nullptr;
    s.entries.count = 0;
    s.entries.capacity = 0;
    s.entries.step = 10;
    g_unk_716e2a76b0 = reinterpret_cast<UnkGfxHeap*>(heap);
    s.entries.field_0x14 = true;
    s.entries.flags = 0x18;
    s.entries.allocator = fn_7100083990();
    fn_7100000250(fn_7100094798, &s.entries, g_unk_7101a19000);
    base::UnkPairArrays& pairs = g_unk_716e2a9708;
    pairs.keys = nullptr;
    pairs.values = nullptr;
    pairs.count = 0;
    pairs.capacity = 0;
    pairs.step = 10;
    pairs.flags = 0x26;
    pairs.allocator = fn_7100083990();
    fn_7100000250(fn_710015e9a4, &pairs, g_unk_7101a19000);
    UnkSlotInit_b0246* slots = reinterpret_cast<UnkSlotInit_b0246*>(g_unk_716e2a9730);
    UnkSlotInit_b0246* p = slots;
    do {
        p->ptr = 0;
        p->field_0x8 = 0;
        p->state = 2;
        p->field_0x14 = 1;
        p->field_0x18 = 1;
        p->field_0x24 = 2;
        p->field_0x28 = 2;
        p->field_0x2c = 2;
        p->field_0x30 = 1;
        p->field_0x34 = 0;
        p->field_0x38 = 0;
        p->transition = 3;
        p->field_0x20 = 0;
        p->field_0x48 = 1;
        p->field_0x4c = 1;
        p->field_0x50 = 1;
        p->field_0x54 = 0;
        p->field_0x58 = 0;
        p->field_0x5c = 0;
        p->field_0x60 = 0;
        p->field_0x3c = -16.0f;
        p->field_0x40 = 16.0f;
        p->field_0x44 = 15.0f;
        p->field_0x64 = 0;
        p->field_0x68 = 0;
        p->field_0x70 = 1;
    } while (++p != slots + 48);
}

// A uniform block: one buffer per frame (from +0x40) and the handles at +0xb0, the size checked at
// +0x38, a dirty flag.
struct UnkFlushBlock_b0246 {
    u8 pad_0x0[0x30];
    s32 field_0x30;
    u8 pad_0x34[4];
    u64 size;           // 0x38
    u8* buffers[0x12];  // 0x40
    u64 handles[0x12];  // 0xd0 - 0x20: placeholder (the handles for the frames at +0xb0)
};

struct UnkFlushStage_b0246 {
    u8 pad_0x0[0x4c];
    u32 block_count;  // 0x4c
    u8 pad_0x50[0x60];
    u8* blocks;  // 0xb0
    bool dirty;  // 0xb8
};

struct UnkFlushState_b0246 {
    UnkFlushStage_b0246* stages[3];
};

struct UnkGfxProgram;
extern void (*g_unk_716e29ba58)(void* context, void* handle, u64 size, u64 offset, u64 length, void* data);

static inline __attribute__((always_inline)) void FlushStage_b0246(UnkFlushStage_b0246* stage) {
    for (u64 i = 0; i < stage->block_count; ++i) {
        u8* blocks = stage->blocks;
        u8* block = blocks + i * 0xd8;
        if (block[0xd0] && *reinterpret_cast<s32*>(block + 0x30) >= 0) {
            u32 frame = fn_7100153da0();
            void* context = fn_7100153d50();
            u8* slot = blocks + (u64)frame * 8 + i * 0xd8;
            void* handle = *reinterpret_cast<void**>(slot + 0xb0);
            void* data = *reinterpret_cast<void**>(slot + 0x40);
            u64 size = *reinterpret_cast<u64*>(block + 0x38);
            g_unk_716e29ba58(context, handle, size, 0, size, data);
            block[0xd0] = 0;
        }
    }
    stage->dirty = false;
}

static inline UnkFlushState_b0246* State_b0246() { return reinterpret_cast<UnkFlushState_b0246*>(g_unk_716e2a3448); }

void fn_710015e700() {
    if (!((g_unk_716e2a3450 != 0) & (g_unk_716e2a3448 != nullptr)))
        return;
    if (UnkFlushStage_b0246* stage = State_b0246()->stages[0])
        FlushStage_b0246(stage);
    if (UnkFlushStage_b0246* stage = State_b0246()->stages[1])
        FlushStage_b0246(stage);
    if (UnkFlushStage_b0246* stage = State_b0246()->stages[2])
        FlushStage_b0246(stage);
}
