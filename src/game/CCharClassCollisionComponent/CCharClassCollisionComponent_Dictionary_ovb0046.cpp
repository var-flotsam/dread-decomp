// CCharClassCollisionComponent's unit (tools/tu_map.py, unit 1205): the definition's constructor and the
// helpers of its collision-material dictionary (base::global::CRntSmallDictionary<CStrId, EColMat>, named by
// the unit's reflection in src/game/CCharClassCollisionComponent/fn_710079d010.cpp): two parallel arrays,
// u32 materials then string-id keys, sharing a count. fn_710079bb14 and fn_710079c034 are the iterator
// value moves (refl_IterValueMove), fn_710079ccb8 erases at a value slot (the last entry moves into the
// hole) and fn_710079c9ec is the copy assignment. Names and layouts are placeholders.
#include <new>

#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassCollisionComponent.h"
#include "game/CCharClassComponent.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::UnkStrIdMember;

// A string id moved out by taking its data and leaving the pool's empty string.
struct UnkMovedStrId_ovb0046 {
    base::global::UnkStringData* data;

    UnkMovedStrId_ovb0046(UnkMovedStrId_ovb0046&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

__attribute__((flatten)) void fn_710079bb14(void* dst, const void* src) {
    base::refl_IterValueMove<u32, UnkMovedStrId_ovb0046>(dst, src);
}

__attribute__((flatten)) void fn_710079c034(void* dst, const void* src) {
    base::refl_IterValueMove<u32, UnkMovedStrId_ovb0046>(dst, src);
}

// The dictionary: materials, their keys, the shared count, then the arrays' growth settings.
struct UnkColMatDict_ovb0046 {
    u32* values;
    UnkStrIdMember* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkStruct_71008bca30;

struct UnkCollisionDefCtor_ovb0046 {
    void** vtable;
    u8 pad_0x8[0x10];
    u64 field_0x18;
    u8 pad_0x20[0x38];
    u8 field_0x58[0x30];  // CRntString
    bool field_0x88;
    bool field_0x89;
    u8 pad_0x8a[0x2];
    u32 field_0x8c[4];
    u8 pad_0x9c[0x4];
    u32* values;  // 0xa0: the dictionary
    u64* keys;
    u32 count;  // 0xb0
    u32 capacity;
    u32 step;  // 0xb8
    u32 flags;
    base::UnkAllocator* allocator;  // 0xc0
    u16 field_0xc8;
};
static_assert(__builtin_offsetof(UnkCollisionDefCtor_ovb0046, field_0xc8) == 0xc8, "layout");

// The constructor: the base, the vtable, an empty string at +0x58, cleared flags and an empty dictionary
// (growth step 10, flags 0xe, the default allocator), then the post-construction virtual call (slot 3).
void fn_710079cf70(UnkCollisionDefCtor_ovb0046* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = &g_unk_7101a6fd78[2];
    fn_710007e6c4(self->field_0x58);
    self->field_0x88 = false;
    self->field_0x89 = false;
    self->values = nullptr;
    self->keys = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = 10;
    self->flags = 0xe;
    self->field_0x8c[0] = 0;
    self->field_0x8c[1] = 0;
    self->field_0x8c[2] = 0;
    self->field_0x8c[3] = 0;
    self->allocator = fn_7100083990();
    self->field_0x18 = 1000;
    self->field_0xc8 = 0;
    reinterpret_cast<void (*)(UnkCollisionDefCtor_ovb0046*)>(self->vtable[3])(self);
}

// Erases the entry whose material `it` points at; the last entry takes its place. Returns the slot.
u32* fn_710079ccb8(UnkColMatDict_ovb0046* self, u32* it) {
    u32 index = static_cast<u32>(it - self->values);
    self->keys[index].~UnkStrIdMember();
    u32 last = self->count - 1;
    if (last != index) {
        self->values[index] = self->values[last];
        // The key is moved by hand; the emptied id is written untyped (the original reloads the count).
        UnkStrIdMember& from = self->keys[self->count - 1];
        self->keys[index].data = from.data;
        base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&from.data, &empty, sizeof(empty));
        self->keys[self->count - 1].~UnkStrIdMember();
    }
    --self->count;
    return &self->values[index];
}

void fn_710079cb54(UnkColMatDict_ovb0046* self);
void fn_710079c890(UnkColMatDict_ovb0046* self, u32 n);  // reserve

// Copy assignment: releases the keys, makes room (fn_710079cb54, then the reserve) when the capacity
// differs from the other's count, copies the materials and retains the other's keys.
UnkColMatDict_ovb0046* fn_710079c9ec(UnkColMatDict_ovb0046* self, const UnkColMatDict_ovb0046* other) {
    if (self == other)
        return self;
    if (self->keys) {
        for (u64 i = 0; i < self->count; ++i)
            self->keys[i].~UnkStrIdMember();
    }
    self->count = 0;
    if (other->count != self->capacity) {
        fn_710079cb54(self);
        fn_710079c890(self, other->count);
    }
    __builtin_memcpy(self->values, other->values, (u64)other->count * sizeof(u32));
    self->count = other->count;
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        base::global::UnkStringData* d = other->keys[i].data;
        __builtin_memcpy(&self->keys[i], &d, sizeof(d));
        if (&g_unk_7101d47108->empty != d)
            ++d->ref_count;
    }
    return self;
}
