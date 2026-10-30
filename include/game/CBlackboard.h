#pragma once

// CBlackboard: named sections of properties. The name comes from reflection
// (CBlackboard::refl_GetClass: size 0xf0, a "hashSections" member, CreateSection(sKey) and
// AfterSectionsDeserialization functions, and a pointer to CBlackboard::CSection). The game
// manager embeds one at +0x968 (game/UnkGameManager.h) and the user data is another. The sections
// and the ids that name them are two parallel arrays; a lookup is a linear search of the ids and
// a section is created on first use. Which method does what is inferred from the code, so the
// methods keep their addresses as names, and members and layouts are placeholders.

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

// A section's properties as parallel arrays: 24-byte values and the ids that name them. They
// share a count, a capacity, a growth step and the allocation flags.
struct UnkPropertyArrays {
    void* values;
    base::global::CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};
static_assert(sizeof(UnkPropertyArrays) == 0x28, "UnkPropertyArrays placeholder layout");

// Grows both arrays to hold `capacity` entries (a no-op when they already do), moving the
// entries across (fn_7100093dd0 is not decompiled yet).
void fn_7100093dd0(UnkPropertyArrays* self, u32 capacity);

// What a section holds from +0x18: the properties and the capacity they are created with.
struct UnkSectionData {
    u32 reserve;
    UnkPropertyArrays properties;
    u32 flags;
    u32 field_0x34;  // not set by the constructor
    bool field_0x38;
};
static_assert(sizeof(UnkSectionData) == 0x40, "UnkSectionData placeholder layout");

struct CBlackboard {
    struct CSection;

    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    // Adds `section` under `key` (called by fn_71010472e0 on a section it has just created).
    virtual void vf_0x28(const base::global::CStrId& key, CSection* section);

    CSection** sections;
    base::global::CStrId* ids;
    u32 count;
    u8 unk_0x1c[0xcc];
    bool field_0xe8;  // set while the data is locked: fn_7100dc8828's setters leave it alone
};
static_assert(sizeof(CBlackboard) == 0xf0, "CBlackboard placeholder layout");

struct CBlackboard::CSection {
    base::global::CStrId key;
    CBlackboard* blackboard;
    void* field_0x10;
    UnkSectionData data;
};
static_assert(sizeof(CBlackboard::CSection) == 0x58, "CBlackboard::CSection placeholder layout");

// The slot of the section `key` names, or the slot past the last one, inlined into each lookup
// (base/UnkParallelFind.h has the same search for other maps of this shape). The ids are compared
// as raw 64-bit words (the original loads the key after the array); the slot is returned, and
// compared with the end by the callers, as the keyed arrays of base/UnkCloneCalls.h do (written
// otherwise the compiler folds the comparison away).
__attribute__((always_inline)) inline CBlackboard::CSection** UnkBlackboardFindSlot(CBlackboard* self,
                                                                                   const base::global::CStrId& key) {
    for (u64 i = 0; i < self->count; ++i) {
        if (*reinterpret_cast<const u64*>(&self->ids[i]) == *reinterpret_cast<const u64*>(&key))
            return &self->sections[i];
    }
    return &self->sections[self->count];
}

// Whether a section is named by `key` (src/game/CBlackboard.cpp).
bool fn_7101047208(CBlackboard* self, const base::global::CStrId& key);

// The section `key` names, or null (the lookup alone: src/game/CBlackboard.cpp).
CBlackboard::CSection* fn_7101047270(CBlackboard* self, const base::global::CStrId& key);

// The section `key` names; one is created and added through vf_0x28 when there is none
// (fn_71010472e0, src/game/CBlackboard.cpp).
CBlackboard::CSection* fn_71010472e0(CBlackboard* self, const base::global::CStrId& key);

// Clears every section's flag and the flags of the objects on the list at +0x70
// (src/game/CBlackboard.cpp).
void fn_7101047178(CBlackboard* self);

// Copies of the same lookup (src/game/CBlackboard.cpp).
CBlackboard::CSection* fn_7101047414(CBlackboard* self, const base::global::CStrId& key);
CBlackboard::CSection* fn_7101047484(CBlackboard* self, const base::global::CStrId& key);
// Reads a bool from the section's value under `name` into `*out`; returns whether it was found.
bool fn_710006d11c(CBlackboard::CSection* self, const base::global::CStrId* name, bool* out);
