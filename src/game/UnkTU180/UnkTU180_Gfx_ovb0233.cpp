// Unit 180's device layer (tools/tu_map.py has no name for it): a table that grows its two
// arrays (fn_710015d5d8), the creation of a shader program record from up to three stages
// (fn_710015d760) and the release of a program's blocks and tables (fn_710015c770). Names and
// layouts are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "game/UnkTU172.h"
#include "types.h"

using base::global::CRntString;

struct UnkNamedEntry_b0233d3 {
    CRntString name;
    u64 extra[2];  // 0x30
};
static_assert(sizeof(UnkNamedEntry_b0233d3) == 0x40, "layout");

struct UnkNamedTable_b0233d3 {
    UnkNamedEntry_b0233d3* entries;  // 0x0
    u64* values;                     // 0x8
    u32 count;                       // 0x10
    u32 capacity;                    // 0x14
    u8 pad_0x18[4];
    u32 flags;                       // 0x1c
    base::UnkAllocator* allocator;   // 0x20
};

void fn_710015d5d8(UnkNamedTable_b0233d3* self, u32 newCapacity) {
    if (self->capacity >= newCapacity)
        return;
    auto* newEntries = static_cast<UnkNamedEntry_b0233d3*>(
        self->allocator->vf_0x40(static_cast<u64>(newCapacity) << 6, 8, self->flags));
    auto* newValues = static_cast<u64*>(self->allocator->vf_0x40(static_cast<u64>(newCapacity) << 3, 8, self->flags));
    UnkNamedEntry_b0233d3* old = self->entries;
    if (old) {
        UnkNamedEntry_b0233d3* dst = newEntries;
        for (u32 i = 0; i < self->count; i++) {
            fn_710007eab4(&dst->name, &old->name);
            __builtin_memcpy(dst->extra, old->extra, sizeof(dst->extra));
            fn_710007ecd4(&old->name);
            dst++;
            old++;
        }
        self->allocator->vf_0x48(self->entries);
    }
    if (self->values) {
        memcpy(newValues, self->values, static_cast<u64>(self->count) << 3);
        self->allocator->vf_0x48(self->values);
    }
    self->entries = newEntries;
    self->values = newValues;
    self->capacity = newCapacity;
}

struct UnkGfxShader_b0233 {
    u8 pad_0x0[0x18];
    u8 field_0x18[0x3c];
    u32 flags;       // 0x54
    u64 field_0x58;
};

struct UnkGfxDesc_b0233 {
    u64 handle;
    u64 value;
};

struct UnkGfxProgram_b0233d4 {
    UnkGfxShader_b0233* stage_b;  // 0x0
    UnkGfxShader_b0233* stage_c;  // 0x8
    UnkGfxShader_b0233* stage_a;  // 0x10
    u32 flags;                    // 0x18
    s32 index;                    // 0x1c
    u8 sync[0xc0];                // 0x20
    UnkGfxDesc_b0233 descs[3];    // 0xe0
    s64 field_0x110[8];
};
static_assert(sizeof(UnkGfxProgram_b0233d4) == 0x150, "layout");

struct UnkGfxProgram;
extern UnkGfxProgram* g_unk_716e2a1440[0x400];
extern u32 g_unk_716e2a3440;
extern u64 (*g_unk_716e29b088)(void*);
extern void (*g_unk_716e29aef0)(void* sync, void* device);
extern void (*g_unk_716e29af08)(void* sync, u32 count, void* descs);

// Through a wrapper: the arguments are evaluated before the entry point is loaded.
static inline void InitializeSync(void* sync, void* device) {
    g_unk_716e29aef0(sync, device);
}

void fn_710015d760(UnkGfxProgram_b0233d4** out, UnkGfxShader_b0233* a, UnkGfxShader_b0233* b, UnkGfxShader_b0233* c) {
    *out = nullptr;
    if (((u64)b | (u64)a | (u64)c) == 0)
        return;
    u32 index = g_unk_716e2a3440;
    if (__builtin_expect(index > 0x3fe, 0))
        return;
    UnkGfxProgram_b0233d4* program = reinterpret_cast<UnkGfxProgram_b0233d4*>(g_unk_716e2a1440[index]);
    program->index = index;
    program->stage_b = b;
    program->stage_c = c;
    program->stage_a = a;
    for (s64& v : program->field_0x110)
        v = -1;
    g_unk_716e2a3440 = index + 1;
    *out = program;
    program->flags = 0;
    u32 flags = 0;
    if (b) {
        flags = b->flags;
        program->flags = flags;
    }
    if (c) {
        flags |= c->flags;
        program->flags = flags;
    }
    if (a) {
        flags |= a->flags;
        program->flags = flags;
    }
    for (UnkGfxDesc_b0233& d : program->descs) {
        d.handle = 0;
        d.value = 0;
    }
    u32 n = 0;
    if (b) {
        program->descs[n].handle = g_unk_716e29b088(b->field_0x18);
        program->descs[n].value = b->field_0x58;
        n++;
    }
    if (c) {
        program->descs[n].handle = g_unk_716e29b088(c->field_0x18);
        program->descs[n].value = c->field_0x58;
        n++;
    }
    if (a) {
        program->descs[n].handle = g_unk_716e29b088(a->field_0x18);
        program->descs[n].value = a->field_0x58;
        n++;
    }
    InitializeSync(program->sync, fn_7100153cd0());
    g_unk_716e29af08(program->sync, n, program->descs);
}

struct UnkHeap_71001541a0;
void fn_7100153a88(UnkHeap_71001541a0* pool, void* ptr);
extern void (*g_unk_716e29b078)(void* obj);
extern void* g_unk_716e2a34a0;

// Through a wrapper: the argument is evaluated before the entry point is loaded.
static inline void Release(void* obj) {
    g_unk_716e29b078(obj);
}
extern void* g_unk_716e2a3498;

struct UnkGfxBlock_b0233d6 {
    CRntString name;
    u8 pad_0x30[0x20];
    u8 field_0x50[0x30];
    u8 field_0x80[0x40];
    void* field_0xc0;
    void* field_0xc8;
    u8 pad_0xd0[8];
    ~UnkGfxBlock_b0233d6() {}
};
static_assert(sizeof(UnkGfxBlock_b0233d6) == 0xd8, "layout");

struct UnkGfxVarying_b0233d6 {
    CRntString name;
    u8 pad_0x30[8];
};

struct UnkGfxEntry_b0233d6 {
    CRntString name;
    u64 extra[2];
};

struct UnkGfxRecord_b0233d6 {
    UnkGfxRecord_b0233d6* field_0x0;
    u32 field_0x8;
    u8 pad_0xc[0xc];
    u8 field_0x18[0x30];
    union {
        u64 field_0x48;
        struct {
            u32 pad_0x48;
            u32 blockCount;  // 0x4c
        };
    };
    u32 index;                      // 0x50
    u32 field_0x54;
    u64 field_0x58;
    u8 pad_0x60[8];
    void* field_0x68;
    u32 field_0x70;
    u8 pad_0x74[4];
    UnkGfxEntry_b0233d6* entries;   // 0x78
    u8 pad_0x80[8];
    u32 entryCount;                 // 0x88
    u8 pad_0x8c[0x14];
    UnkGfxVarying_b0233d6* varyings;  // 0xa0
    UnkGfxEntry_b0233d6* field_0xa8;
    UnkGfxBlock_b0233d6* blocks;    // 0xb0
};

struct UnkGfxRecord_0xc0;
extern UnkGfxRecord_0xc0* g_unk_716e29d438[];
extern u32 g_unk_716e2a1438;

void fn_710015c770(UnkGfxRecord_b0233d6* self) {
    if (!self)
        return;
    for (u32 i = 0; i < self->blockCount; i++) {
        UnkGfxBlock_b0233d6* block = &self->blocks[i];
        Release(block->field_0x50);
        fn_7100153a88(static_cast<UnkHeap_71001541a0*>(g_unk_716e2a34a0), block->field_0xc0);
        block->field_0xc0 = nullptr;
        Release(block->field_0x80);
        fn_7100153a88(static_cast<UnkHeap_71001541a0*>(g_unk_716e2a34a0), block->field_0xc8);
        block->field_0xc8 = nullptr;
    }
    delete[] self->blocks;
    self->blocks = nullptr;
    self->field_0x58 = 0;
    delete[] self->varyings;
    self->varyings = nullptr;
    self->field_0x48 = 0;
    self->field_0x70 = 0;
    self->field_0x54 = 0;
    Release(self->field_0x18);
    fn_7100153a88(static_cast<UnkHeap_71001541a0*>(g_unk_716e2a3498), self->field_0x68);
    self->field_0x68 = nullptr;
    self->field_0x8 = 0;
    self->field_0x0 = nullptr;
    delete[] self->field_0xa8;
    self->field_0xa8 = nullptr;
    if (self->entries) {
        for (u32 i = 0; i < self->entryCount; i++)
            self->entries[i].name.~CRntString();
    }
    u32 index = self->index;
    self->entryCount = 0;
    if (index <= 0x7ff) {
        u32 last = g_unk_716e2a1438 - 1;
        UnkGfxRecord_b0233d6* moved = reinterpret_cast<UnkGfxRecord_b0233d6*>(g_unk_716e29d438[last]);
        g_unk_716e2a1438 = last;
        g_unk_716e29d438[index] = reinterpret_cast<UnkGfxRecord_0xc0*>(moved);
        moved->index = index;
        g_unk_716e29d438[last] = reinterpret_cast<UnkGfxRecord_0xc0*>(self);
        self->index = -1;
    }
}
