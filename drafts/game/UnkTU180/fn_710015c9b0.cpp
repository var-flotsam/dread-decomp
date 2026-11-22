// Unit 180's device layer: builds a shader record from one stage of a compiled shader binary (its code
// buffer, its uniform blocks and uniforms and, for the vertex stage, its attributes).
// Names and layouts are placeholders.
#include <stdio.h>
#include <string.h>

#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU172.h"
#include "game/UnkTU176.h"
#include "game/UnkTU180.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

struct UnkGfxHeap {
    u8 pool[0x100];
    u64 base;  // 0x100
    u8 pad_0x108[0x18];
    void* space;  // 0x120
};

void* fn_7100153a10(UnkGfxHeap* heap, void* old, u64 size, u64 align);
void* fn_7100153dd0();
u64 fn_710015383c(u64 size, u64 align);
u32 fn_710015d700(u32 type);  // a uniform type's size in bytes
struct UnkNamedTable_b0233d3;
void fn_710015d5d8(UnkNamedTable_b0233d3* self, u32 newCapacity);

extern void (*g_unk_716e29b038)(void* builder);
extern void (*g_unk_716e29b040)(void* builder, void* pool, u64 offset, u64 size);
extern void (*g_unk_716e29b068)(void* buffer, void* builder);
extern void* (*g_unk_716e29b080)(void* buffer);
extern u64 (*g_unk_716e29b088)(void*);
extern void* g_unk_716e2a3498;
extern void* g_unk_716e2a34a0;
struct UnkGfxRecord_0xc0;
extern UnkGfxRecord_0xc0* g_unk_716e29d438[];

// The binary's tables.
struct UnkShaderSection_l15c9b0 {
    u8 pad_0x0[4];
    u32 offset;  // 0x4: from the start of the binary
    u32 type;    // 0x8: 0 code, 3 reflection
    u8 pad_0xc[0x20];
    u32 field_0x2c;  // code: the stage; reflection: the block count
    u32 field_0x30;  // code: the code offset; reflection: the block table
    u32 field_0x34;  // code: the data offset; reflection: the uniform count
    u32 field_0x38;  // code: the size; reflection: the uniform table
    u32 field_0x3c;  // reflection: the attribute count
    u32 field_0x40;  // reflection: the attribute table
    u8 pad_0x44[0x24];
    u32 field_0x68;  // reflection: the string table
    u8 pad_0x6c[0x24];
};
static_assert(sizeof(UnkShaderSection_l15c9b0) == 0x90, "layout");

struct UnkShaderBinary_l15c9b0 {
    u8 pad_0x0[0x4c];
    u32 sectionCount;  // 0x4c
    u8 pad_0x50[0x40];
    UnkShaderSection_l15c9b0 sections[1];  // 0x90
};

struct UnkShaderBlockDesc_l15c9b0 {
    u32 nameOffset;
    u8 pad_0x4[0x24];
    u32 size;  // 0x28
    u8 pad_0x2c[8];
    s32 bindings[3];  // 0x34: by stage
    u8 pad_0x40[0x2c];
};
static_assert(sizeof(UnkShaderBlockDesc_l15c9b0) == 0x6c, "layout");

struct UnkShaderUniformDesc_l15c9b0 {
    u32 nameOffset;
    u8 pad_0x4[0x24];
    u32 type;       // 0x28
    u32 block;      // 0x2c
    s32 offset;     // 0x30: -1 when none
    u32 arraySize;  // 0x34
    u8 pad_0x38[0x2d];
    u8 isArray;  // 0x65
    u8 pad_0x66[0x3e];
};
static_assert(sizeof(UnkShaderUniformDesc_l15c9b0) == 0xa4, "layout");

struct UnkShaderAttributeDesc_l15c9b0 {
    u32 nameOffset;
    u8 pad_0x4[0x28];
    u32 arraySize;  // 0x2c
    s32 location;   // 0x30
    u8 pad_0x34[4];
    u8 isArray;  // 0x38
    u8 pad_0x39[0x23];
};
static_assert(sizeof(UnkShaderAttributeDesc_l15c9b0) == 0x5c, "layout");

// The record's tables.
struct UnkShaderUniformInfo_l15c9b0 {
    u32 offset;
    u32 size;
    u32 block;
    u32 index;
};

struct UnkShaderUniform_l15c9b0 {
    CRntString name;
    UnkShaderUniformInfo_l15c9b0 info;  // 0x30

    void Init() {
        fn_710007e6c4(&name);
        info = {};
    }
};
static_assert(sizeof(UnkShaderUniform_l15c9b0) == 0x40, "layout");

struct UnkShaderAttribute_l15c9b0 {
    CRntString name;
    s32 location;  // 0x30

    void Init() {
        fn_710007e6c4(&name);
        location = -1;
    }
};
static_assert(sizeof(UnkShaderAttribute_l15c9b0) == 0x38, "layout");

struct UnkShaderBlock_l15c9b0 {
    CRntString name;
    s32 binding;  // 0x30
    u8 pad_0x34[4];
    u64 size;              // 0x38
    void* mapped[2];       // 0x40
    u8 buffers[2][0x30];   // 0x50
    u64 addresses[2];      // 0xb0
    void* memory[2];       // 0xc0
    bool field_0xd0;
    bool raw;  // 0xd1: named _RAWCB_

    void Init() {
        fn_710007e6c4(&name);
        binding = -1;
        field_0xd0 = true;
        raw = false;
        size = 0;
        mapped[0] = nullptr;
        mapped[1] = nullptr;
        memory[0] = nullptr;
        memory[1] = nullptr;
    }
};
static_assert(sizeof(UnkShaderBlock_l15c9b0) == 0xd8, "layout");

// The uniforms by name key (grown by fn_710015d5d8).
struct UnkShaderUniformTable_l15c9b0 {
    UnkShaderUniform_l15c9b0* entries;  // 0x0
    s64* keys;                          // 0x8
    u32 count;                          // 0x10
    u32 capacity;                       // 0x14
    u32 step;                           // 0x18
    u32 flags;                          // 0x1c
    void* allocator;                    // 0x20

    void Reserve(u32 newCapacity) {
        fn_710015d5d8(reinterpret_cast<UnkNamedTable_b0233d3*>(this), newCapacity);
    }

    void Add(const UnkShaderUniform_l15c9b0* uniform) {
        s64 key = uniform->name.key;
        if (count == capacity)
            Reserve(step + count);
        keys[count] = key;
        UnkShaderUniform_l15c9b0* entry = &entries[count];
        CRntString_CopyConstruct(reinterpret_cast<base::UnkNamedEntry*>(&entry->name),
                      reinterpret_cast<const base::UnkNamedEntry*>(&uniform->name));
        entry->info = uniform->info;
        count++;
    }
};

struct UnkShaderRecord_l15c9b0 {
    u8* data;  // 0x0
    u32 size;  // 0x8
    u8 pad_0xc[4];
    u64 field_0x10;
    u8 buffer[0x30];         // 0x18
    u32 uniformCount;        // 0x48
    u32 blockCount;          // 0x4c
    s32 index;               // 0x50
    u32 stageKind;           // 0x54
    u8* code;                // 0x58
    u64 field_0x60;
    void* memory;            // 0x68
    u32 attributeCount;      // 0x70
    u32 field_0x74;
    UnkShaderUniformTable_l15c9b0 table;     // 0x78
    UnkShaderAttribute_l15c9b0* attributes;  // 0xa0
    UnkShaderUniform_l15c9b0* uniforms;      // 0xa8
    UnkShaderBlock_l15c9b0* blocks;          // 0xb0
    bool field_0xb8;
};
static_assert(__builtin_offsetof(UnkShaderRecord_l15c9b0, field_0xb8) == 0xb8, "layout");

// The arguments are evaluated before the entry point is loaded.
template <class... P, class... A>
static inline void CallNvn(void (*&fn)(P...), A... args) {
    fn(args...);
}

static inline UnkGfxHeap* CodeHeap() {
    return static_cast<UnkGfxHeap*>(g_unk_716e2a3498);
}

static inline UnkGfxHeap* BlockHeap() {
    return static_cast<UnkGfxHeap*>(g_unk_716e2a34a0);
}

// new T[count], with its count before the elements.
template <class T>
static inline T* NewArray(u32 count) {
    u64* mem = static_cast<u64*>(fn_7100083ba0(count * sizeof(T) + 8, 0x2f));
    *mem = count;
    T* items = reinterpret_cast<T*>(mem + 1);
    for (T* it = items; it != items + count; it++)
        it->Init();
    return items;
}

// Samplers and images, which take no room in a uniform block.
static inline bool IsOpaqueType(u32 type) {
    switch (type) {
    case 0x43:
    case 0x45:
    case 0x47:
    case 0x67:
        return true;
    default:
        return false;
    }
}

// One of a block's two buffers, in memory of its own from the block heap.
static inline void CreateBlockBuffer(UnkShaderBlock_l15c9b0* block, u32 k, u64 size) {
    block->memory[k] = fn_7100153a10(BlockHeap(), nullptr, size, 0x1000);
    CallNvn(g_unk_716e29b040, fn_7100153dd0(), static_cast<void*>(BlockHeap()->pool),
            reinterpret_cast<u64>(block->memory[k]) - BlockHeap()->base, block->size);
    CallNvn(g_unk_716e29b068, static_cast<void*>(block->buffers[k]), fn_7100153dd0());
    block->addresses[k] = g_unk_716e29b088(block->buffers[k]);
    block->mapped[k] = g_unk_716e29b080(block->buffers[k]);
}

static inline void AddUniform(UnkShaderRecord_l15c9b0* rec, const char* name, u32 offset, u32 size,
                              const UnkShaderUniformDesc_l15c9b0* desc) {
    UnkShaderUniform_l15c9b0* uniform = &rec->uniforms[rec->uniformCount];
    CRntString_AssignCString(&uniform->name, name);
    uniform->info.size = size;
    uniform->info.offset = offset;
    uniform->info.block = desc->block;
    uniform->info.index = rec->uniformCount;
    rec->uniformCount++;
    rec->table.Add(uniform);
}

void fn_710015c9b0(UnkShaderRecord_l15c9b0** out, u32 stage, u8* binary) {
    u32 slot = g_unk_716e2a1438;
    if (slot > 0x7fe)
        return;
    UnkShaderRecord_l15c9b0* rec = reinterpret_cast<UnkShaderRecord_l15c9b0*>(g_unk_716e29d438[slot]);
    rec->data = nullptr;
    rec->size = 0;
    rec->field_0x10 = 0;
    rec->uniformCount = 0;
    rec->blockCount = 0;
    rec->index = -1;
    rec->stageKind = 0;
    rec->code = nullptr;
    rec->field_0x60 = 0;
    rec->memory = nullptr;
    rec->attributeCount = 0;
    rec->field_0x74 = 0;
    if (rec->table.entries) {
        for (u32 i = 0; i < rec->table.count; i++)
            rec->table.entries[i].name.~CRntString();
    }
    rec->table.count = 0;
    rec->attributes = nullptr;
    rec->uniforms = nullptr;
    rec->blocks = nullptr;
    rec->field_0xb8 = true;

    u32 kind = 1;
    rec->index = g_unk_716e2a1438;
    g_unk_716e2a1438++;
    *out = rec;
    if (stage == 2)
        kind = 4;
    else if (stage == 1)
        kind = 2;
    else
        stage = 0;

    s32 alignment = 0;
    CallNvn(g_unk_716e29ac78, fn_7100153cd0(), 4, &alignment);
    rec->uniformCount = 0;
    rec->blockCount = 0;

    UnkShaderBinary_l15c9b0* header = reinterpret_cast<UnkShaderBinary_l15c9b0*>(binary);
    u32 sectionCount = header->sectionCount;
    for (u32 i = 0; i < sectionCount; i++) {
        UnkShaderSection_l15c9b0* section = &header->sections[i];
        if (section->type == 0) {
            if (section->field_0x2c != stage)
                continue;
            rec->stageKind = kind;
            u8* base = binary + section->offset;
            rec->size = section->field_0x38;
            rec->data = base + section->field_0x34;
            rec->code = base + section->field_0x30;
            u64 size = fn_710015383c(section->field_0x38 + 0x2000ul, alignment);
            CallNvn(g_unk_716e29b038, fn_7100153dd0());
            rec->memory = fn_7100153a10(CodeHeap(), rec->data, size, 0x1000);
            CallNvn(g_unk_716e29b040, fn_7100153dd0(), static_cast<void*>(CodeHeap()->pool),
                    reinterpret_cast<u64>(rec->memory) - CodeHeap()->base, static_cast<u64>(rec->size));
            CallNvn(g_unk_716e29b068, static_cast<void*>(rec->buffer), fn_7100153dd0());
        } else if (section->type == 3) {
            u8* base = binary + section->offset;
            u32 blockCount = section->field_0x2c;
            UnkShaderBlockDesc_l15c9b0* blockDesc =
                reinterpret_cast<UnkShaderBlockDesc_l15c9b0*>(base + section->field_0x30);
            const char* strings = reinterpret_cast<const char*>(base) + section->field_0x68;
            rec->blockCount = blockCount;
            rec->blocks = NewArray<UnkShaderBlock_l15c9b0>(blockCount);
            for (u32 j = 0; j < rec->blockCount; j++, blockDesc++) {
                UnkShaderBlock_l15c9b0* block = &rec->blocks[j];
                block->size = blockDesc->size;
                u64 size = fn_710015383c(blockDesc->size, alignment);
                if (size != 0) {
                    CallNvn(g_unk_716e29b038, fn_7100153dd0());
                    CreateBlockBuffer(block, 0, size);
                    CreateBlockBuffer(block, 1, size);
                }
                CRntString_AssignCString(&block->name, strings + blockDesc->nameOffset);
                if (CRntString_Contains(&block->name, "_RAWCB_"))
                    block->raw = true;
                block->binding = blockDesc->bindings[stage];
            }

            UnkShaderUniformDesc_l15c9b0* uniformDescs =
                reinterpret_cast<UnkShaderUniformDesc_l15c9b0*>(base + section->field_0x38);
            u32 uniformCount = 0;
            for (u32 k = 0; k < section->field_0x34; k++) {
                if (!IsOpaqueType(uniformDescs[k].type)) {
                    uniformCount++;
                    if (uniformDescs[k].isArray)
                        uniformCount += uniformDescs[k].arraySize;
                }
            }
            rec->uniforms = NewArray<UnkShaderUniform_l15c9b0>(uniformCount);
            rec->table.Reserve(uniformCount);

            char baseName[0x400];
            char element[0x400];
            UnkShaderUniformDesc_l15c9b0* desc = uniformDescs;
            for (u32 k = 0; k < section->field_0x34; k++, desc++) {
                if (IsOpaqueType(desc->type))
                    continue;
                if (rec->blocks[desc->block].raw)
                    continue;
                u32 size = fn_710015d700(desc->type);
                if (size == 0)
                    continue;
                const char* name = strings + desc->nameOffset;
                if (desc->isArray) {
                    // "name[0]": the first element under the bare name, then each element.
                    CRntString full(name);
                    s32 bracket = CRntString_FindLastChar(&full, '[', -1);
                    strncpy(baseName, full.data ? full.data : g_unk_710162e2ed, bracket);
                    baseName[bracket] = '\0';
                    AddUniform(rec, baseName, desc->offset == -1 ? 0 : desc->offset, size, desc);
                    for (u32 m = 0; m < desc->arraySize; m++) {
                        sprintf(element, "%s[%u]", baseName, m);
                        AddUniform(rec, element, desc->offset + m * size, size, desc);
                    }
                } else {
                    AddUniform(rec, name, desc->offset == -1 ? 0 : desc->offset, size, desc);
                }
            }

            if (stage == 0) {
                UnkShaderAttributeDesc_l15c9b0* attributeDescs =
                    reinterpret_cast<UnkShaderAttributeDesc_l15c9b0*>(base + section->field_0x40);
                u32 attributeCount = 0;
                for (u32 k = 0; k < section->field_0x3c; k++)
                    attributeCount += attributeDescs[k].isArray ? attributeDescs[k].arraySize + 1 : 2;
                rec->attributes = NewArray<UnkShaderAttribute_l15c9b0>(attributeCount);

                u32 index = 0;
                UnkShaderAttributeDesc_l15c9b0* attribute = attributeDescs;
                for (u32 k = 0; k < section->field_0x3c; k++, attribute++) {
                    const char* name = strings + attribute->nameOffset;
                    CRntString_AssignCString(&rec->attributes[index].name, name);
                    rec->attributes[index].location = attribute->location;
                    // "name[0]": the other elements are "name[1]" and on.
                    u32 m = 1;
                    if (attribute->isArray == 1) {
                        for (; m < attribute->arraySize; m++) {
                            u32 length = strlen(name) - 2;
                            strcpy(baseName, name);
                            baseName[length] = '\0';
                            sprintf(element, "%s%d]", baseName, m);
                            CRntString_AssignCString(&rec->attributes[index + m].name, element);
                            rec->attributes[index + m].location = m + attribute->location;
                        }
                    }
                    index += m;
                }
                rec->attributeCount = index;
            }
        }
    }
}
