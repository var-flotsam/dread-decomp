// Unit 271 of tools/tu_map.py (base::reflection names): removes an object from the registry's
// chained hash maps keyed by the CRC-64 of its name (bucket = (high ^ low) % capacity, as in
// src/base/reflection/UnkUnit271_MapFind_A1_b71.cpp): from the map of all objects (+0x30,
// fn_71001efc90), then by its kind (vtable slot 0x88): kind 2 from the map at +0x78
// (fn_71001efddc), kind 0x10 from the one at +0xc0 (fn_71001eff28). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkRegNode_ovb0108 {
    u64 key;
    u8 pad_0x8[0x18];
    UnkRegNode_ovb0108* chain;  // 0x20
};

struct UnkRegMap_ovb0108 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkRegNode_ovb0108** buckets;  // 0x10 (base::global::CRntVector)
    u32 bucket_count;              // 0x18
    u32 bucket_capacity;           // 0x1c
    u8 pad_0x20[0x28];

    __attribute__((always_inline)) bool Contains(u64 key) {
        if (bucket_count == 0)
            return false;
        u32 i = ((u32)(key >> 32) ^ (u32)key) % bucket_capacity;
        if (bucket_count <= i)
            __assert_fail("i < m_iSize", "..\Base/global/RntVector.h", 0x15b, "operator[]");
        UnkRegNode_ovb0108** bucket = &buckets[i];
        if (!bucket)
            return false;
        for (UnkRegNode_ovb0108* n = *bucket; n; n = n->chain) {
            if (n->key == key)
                return true;
        }
        return false;
    }
};
static_assert(sizeof(UnkRegMap_ovb0108) == 0x48, "UnkRegMap_ovb0108 layout");

void* fn_71001efc90(UnkRegMap_ovb0108* self, const u64* key);
void* fn_71001efddc(UnkRegMap_ovb0108* self, const u64* key);
void* fn_71001eff28(UnkRegMap_ovb0108* self, const u64* key);

struct UnkRegistry_ovb0108 {
    u8 pad_0x0[0x30];
    UnkRegMap_ovb0108 all;      // 0x30
    UnkRegMap_ovb0108 kind2;    // 0x78
    UnkRegMap_ovb0108 kind16;   // 0xc0
};

struct UnkRegNameData_ovb0108 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x44 - 0x18];
    union {
        u32 use_count;
    };
};

struct UnkRegKind_ovb0108 {
    s32 kind;
};

struct UnkRegObject_ovb0108 {
    void** vtable;
    u8 pad_0x8[0x8];
    union {
        UnkRegNameData_ovb0108* name;  // 0x10
    };

    const UnkRegKind_ovb0108* vf_0x88() {
        return reinterpret_cast<const UnkRegKind_ovb0108* (*)(UnkRegObject_ovb0108*)>(vtable[0x88 / 8])(this);
    }
};

void fn_71001eed70(UnkRegistry_ovb0108* self, UnkRegObject_ovb0108* obj) {
    ++obj->name->use_count;
    const char* text = obj->name->text;
    text = text ? text : g_unk_710162e2ed;
    u64 hash = fn_7100001570(text, __builtin_strlen(text));
    if (self->all.Contains(hash))
        fn_71001efc90(&self->all, &hash);
    switch (obj->vf_0x88()->kind) {
    case 2:
        if (self->kind2.Contains(hash))
            fn_71001efddc(&self->kind2, &hash);
        break;
    case 0x10:
        if (self->kind16.Contains(hash))
            fn_71001eff28(&self->kind16, &hash);
        break;
    }
}
