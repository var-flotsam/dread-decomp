// The device layer's object pools (unit 180 of tools/tu_map.py, 0x710015c520-0x710015e4ac): two
// tables of pre-allocated records (2048 of 0xc0 bytes, 1024 of 0x150 bytes, the second holding
// shader programs: fn_710015c690 releases one), a sync object and a memory pool, all created by
// fn_710015c520 during the device set-up (fn_71001587f0); and the uniform setter fn_710015e1f0.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

// The default allocator (base/UnkDefaultAllocator.h) up to its slot 0x90: the free size, summed over
// its sub-allocators (as in src/engine/UnkGfxBlockPool.cpp).
struct UnkGfxObjectPoolAllocator {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual u64 vf_0x90();
};

static inline UnkGfxObjectPoolAllocator* GfxObjectPoolAllocator() {
    return reinterpret_cast<UnkGfxObjectPoolAllocator*>(fn_7100083990());
}

// A record of the first table. +0x90-+0xb8 look like an array (growth step 10, allocator flags 0xe).
struct UnkGfxRecord_0xc0 {
    u64 field_0x0;
    u32 field_0x8;
    u64 field_0x10;
    u8 unk_0x18[0x30];
    u64 field_0x48;
    s32 field_0x50;
    u32 field_0x54[15];
    u32 field_0x90;
    u32 field_0x94;
    base::UnkAllocator* field_0x98;
    u64 field_0xa0;
    u64 field_0xa8;
    u64 field_0xb0;
    bool field_0xb8;

    UnkGfxRecord_0xc0()
        : field_0x0(0), field_0x8(0), field_0x10(0), field_0x48(0), field_0x50(-1), field_0x54(),
          field_0x90(10), field_0x94(0xe), field_0x98(fn_7100083990()), field_0xa0(0), field_0xa8(0),
          field_0xb0(0), field_0xb8(true) {}
};
static_assert(sizeof(UnkGfxRecord_0xc0) == 0xc0, "UnkGfxRecord_0xc0 placeholder layout");

// A record of the second table (a shader program).
struct UnkGfxProgram {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    s32 field_0x1c;
    u8 unk_0x20[0x130];

    UnkGfxProgram() : field_0x0(0), field_0x8(0), field_0x10(0), field_0x18(0), field_0x1c(-1) {}
};
static_assert(sizeof(UnkGfxProgram) == 0x150, "UnkGfxProgram placeholder layout");

struct UnkPool;  // the memory pool type of src/game_s7/fn_71001587f0.cpp

// The pools (all reached through the GOT).
extern UnkGfxRecord_0xc0* g_unk_716e29d438[/* 0x800; unsized: x86-64 would assume 16-byte alignment */];
extern u32 g_unk_716e2a1438;  // live records of the first table
extern UnkGfxProgram* g_unk_716e2a1440[0x400];
extern u32 g_unk_716e2a3440;           // live programs
extern UnkGfxProgram* g_unk_716e2a3448;  // the bound program
extern u8 g_unk_716e2a3458[];          // a sync object
extern UnkPool g_unk_716e29d318;

void* fn_7100153cd0();  // the device
void fn_710015385c(UnkPool* pool, u64 size, s32 flags, void* device);
// NVN's sync initialiser (as src/game_s7/fn_71001587f0.cpp declares it).
extern void (*g_unk_716e29bbb0)(void* sync, void* device);

// Through a wrapper: the arguments are evaluated before the entry point is loaded.
static inline void InitializeSync(void* sync, void* device) {
    g_unk_716e29bbb0(sync, device);
}

// Creates the pools. The free size is read before and after (the results are unused).
void fn_710015c520() {
    GfxObjectPoolAllocator()->vf_0x90();
    for (u32 i = 0; i < 0x800; ++i)
        g_unk_716e29d438[i] = new (fn_71000839a0(sizeof(UnkGfxRecord_0xc0), 0x47)) UnkGfxRecord_0xc0;
    g_unk_716e2a1438 = 0;
    for (UnkGfxProgram*& p : g_unk_716e2a1440)
        p = new (fn_71000839a0(sizeof(UnkGfxProgram), 0x47)) UnkGfxProgram;
    g_unk_716e2a3440 = 0;
    g_unk_716e2a3448 = nullptr;
    GfxObjectPoolAllocator()->vf_0x90();
    InitializeSync(g_unk_716e2a3458, fn_7100153cd0());
    fn_710015385c(&g_unk_716e29d318, 0x100000, 0xa1, fn_7100153cd0());
}

// Three pointers selected by an index (0-2).
struct UnkGfxTriple {
    void* field_0x0;
    void* field_0x8;
    void* field_0x10;
};

// The pointer `which` selects, or null.
void* fn_710015d720(const UnkGfxTriple* self, s32 which) {
    if (!self)
        return nullptr;
    switch (which) {
    case 0:
        return self->field_0x0;
    case 1:
        return self->field_0x8;
    case 2:
        return self->field_0x10;
    }
    return nullptr;
}

// ---- fn_710015e1f0: setting a uniform's values ----

#include <string.h>

u32 fn_7100153da0();          // the current frame's buffer index
extern u8 g_unk_716e2a3450;   // set when any uniform block changed

// A uniform block: one buffer per frame (from +0x40), the size checked at +0x38, a dirty flag.
struct UnkGfxUniformBlock {
    u8 pad_0x0[0x38];
    void* field_0x38;
    u8* buffers[0x12];  // 0x40
    bool dirty;         // 0xd0
    u8 pad_0xd1[0x7];
};
static_assert(sizeof(UnkGfxUniformBlock) == 0xd8, "UnkGfxUniformBlock layout");

// A uniform: its offset (+0x30) and size (+0x34) in its block, and the block's index (+0x38).
struct UnkGfxUniform {
    u8 pad_0x0[0x30];
    u32 offset;
    u32 size;
    u32 block;
    u8 pad_0x3c[0x4];
};

struct UnkGfxStageProgram {
    u8 pad_0x0[0xa8];
    UnkGfxUniform* uniforms;      // 0xa8, by location - 15
    UnkGfxUniformBlock* blocks;   // 0xb0
    bool dirty;                   // 0xb8
};

// Copies `count` vectors (up to the uniform's size) into the current frame's buffer of the uniform
// at `location` of the stage's program (0 to 2 select it in the state), and marks it dirty.
// NON_MATCHING(minor): the original tests the location before the state, selects the stage's slot by
// address (stages 2, 1, then 0) and computes the index before the selection; Clang orders and
// schedules these differently. The behaviour is the same.
void fn_710015e1f0(void* state, u32 stage, u64 location, u32 count, const f32* values) {
    if (!location || !state)
        return;
    u64 index = (location - 15) & 0xffffffff;
    UnkGfxStageProgram* const* slot = static_cast<UnkGfxStageProgram* const*>(state);
    UnkGfxStageProgram* program;
    switch (stage) {
    case 2:
        program = slot[2];
        break;
    case 1:
        program = slot[1];
        break;
    case 0:
        program = slot[0];
        break;
    default:
        program = nullptr;
        break;
    }
    asm("" : "+r"(program));  // the original reads it unchecked
    UnkGfxUniformBlock* blocks = program->blocks;
    UnkGfxUniform* uniform = &program->uniforms[index];
    u32 b = uniform->block;
    if (blocks[b].field_0x38) {
        u32 frame = fn_7100153da0();
        UnkGfxUniformBlock* block = &blocks[b];
        u32 bytes = count << 2;
        memcpy(block->buffers[frame] + uniform->offset, values, bytes < uniform->size ? bytes : uniform->size);
        block->dirty = true;
        program->dirty = true;
        g_unk_716e2a3450 = 1;
    }
}
