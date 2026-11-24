// Unit 259 (the graphics system): the graphics system's destructor body, its members destroyed
// last to first. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

extern void* unk_7101a1e6e0[];

// Out-of-line destructors and clears of members.
template <class Value>
struct UnkIdTable;
struct UnkResWord;
void ClearStrIdKeyedCountedResourceTable(UnkIdTable<UnkResWord>* self);
void fn_71001c943c(char* map);

// Members destroyed out of line.
struct UnkGfxMember_7100216ef8;
void fn_7100216ef8(UnkGfxMember_7100216ef8* self);
struct UnkGfxMember_7100216ef8 {
    u8 unk_0x0[0x40];

    ~UnkGfxMember_7100216ef8() { fn_7100216ef8(this); }
};
struct UnkGfxMember_7100168f28;
void fn_7100168f28(UnkGfxMember_7100168f28* self);
struct UnkGfxMember_7100168f28 {
    u8 unk_0x0[0x18];

    ~UnkGfxMember_7100168f28() { fn_7100168f28(this); }
};

// A hash map, cleared out of line.
struct UnkGfxMap_l1c9070 {
    u8 unk_0x0[0x228];

    ~UnkGfxMap_l1c9070() { fn_71001c943c(reinterpret_cast<char*>(this)); }
};

// A counted reference to a resource.
struct UnkGfxResRef_l1c9070 {
    UnkCountedResource* ptr;

    ~UnkGfxResRef_l1c9070() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }
};

// A vector that frees its array when destroyed.
template <class T>
struct UnkOwnedVector_l1c9070 : CRntVector<T> {
    ~UnkOwnedVector_l1c9070() { this->Free(); }
};

// A named vector (0x18 bytes).
struct UnkGfxNamedVector_l1c9070 {
    CStrId name;
    base::math::CVector4D value;  // 0x8
};

// Named vectors in an array parallel to their 64-bit keys.
struct UnkGfxNamedVectors_l1c9070 {
    UnkGfxNamedVector_l1c9070* values;
    u64* keys;  // 0x8
    u32 count;  // 0x10
    u32 capacity;
    u32 step;
    u32 flags;  // 0x1c
    base::IAllocator* allocator;  // 0x20

    ~UnkGfxNamedVectors_l1c9070() {
        if (values && count) {
            for (u64 i = 0; i < count; ++i)
                values[i].~UnkGfxNamedVector_l1c9070();
        }
        count = 0;
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

// The table at +0x3183628: the same layout, cleared out of line.
struct UnkGfxIdTable_l1c9070 {
    void* values;
    void* keys;  // 0x8
    u32 count;  // 0x10
    u32 capacity;
    u32 step;
    u32 flags;  // 0x1c
    base::IAllocator* allocator;  // 0x20

    ~UnkGfxIdTable_l1c9070() {
        ClearStrIdKeyedCountedResourceTable(reinterpret_cast<UnkIdTable<UnkResWord>*>(this));
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

// A display mode the device reports (plain data).
struct UnkGfxDisplayMode_l1c9070 {
    char name[0x80];
    u32 field_0x80;
    u32 field_0x84;
    u32 field_0x88;
};

// A resource kept under a name.
struct UnkGfxNamedRes_l1c9070 {
    CStrId name;
    UnkGfxResRef_l1c9070 resource;  // 0x8
};

// A count-prefixed list of up to 16 pairs of words and one 64-bit value; destroying it empties it.
struct UnkGfxPairList_l1c9070 {
    u32 count;
    u32 items[16][2];  // 0x4
    u64 field_0x88;

    ~UnkGfxPairList_l1c9070() { count = 0; }
};
static_assert(sizeof(UnkGfxPairList_l1c9070) == 0x90, "pair list size");

struct UnkGfxDtor_l1c9070 {
    void** vtable;
    UnkGfxNamedVectors_l1c9070 named_vectors;  // 0x8
    u8 pad_0x30[0x30];
    UnkOwnedVector_l1c9070<UnkGfxDisplayMode_l1c9070> display_modes;  // 0x60
    UnkOwnedVector_l1c9070<UnkGfxNamedRes_l1c9070> named_resources;  // 0x88
    UnkOwnedVector_l1c9070<UnkGfxResRef_l1c9070> shader_resources;  // 0xb0
    u8 pad_0xd8[0x317a988 - 0xd8];
    UnkGfxMember_7100168f28 field_0x317a988;
    UnkGfxMember_7100216ef8 field_0x317a9a0;
    UnkOwnedVector_l1c9070<UnkGfxPairList_l1c9070> pair_lists;  // 0x317a9e0
    u8 pad_0x317aa08[0x68];
    UnkGfxMap_l1c9070 map;  // 0x317aa70
    UnkGfxResRef_l1c9070 field_0x317ac98;
    u8 pad_0x317aca0[0x2e0];
    CStrId field_0x317af80;
    u8 pad_0x317af88[0x10];
    UnkGfxResRef_l1c9070 field_0x317af98;
    u8 pad_0x317afa0[0x3183628 - 0x317afa0];
    UnkGfxIdTable_l1c9070 field_0x3183628;

    // The graphics system's destructor.
    ~UnkGfxDtor_l1c9070() asm("fn_71001c9070");
};
static_assert(__builtin_offsetof(UnkGfxDtor_l1c9070, field_0x317af98) == 0x317af98, "layout");
static_assert(__builtin_offsetof(UnkGfxDtor_l1c9070, field_0x3183628) == 0x3183628, "layout");

UnkGfxDtor_l1c9070::~UnkGfxDtor_l1c9070() { vtable = &unk_7101a1e6e0[2]; }
