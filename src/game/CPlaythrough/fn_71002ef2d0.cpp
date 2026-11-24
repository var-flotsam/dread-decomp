// Unit 424 (CPlaythrough's neighbours): two parallel arrays (owned records, and a string id per
// record) sharing a count, a capacity and allocation flags. Grows both to a new capacity.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// An object owned through a pointer and destroyed through its vtable (slot 0x10).
struct UnkOwnedObj_l2ef2d0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkRecord_l2ef2d0 {
    u64 field_0x0;
    CStrId name;
    u8 pad_0x10[0x20];
    UnkOwnedObj_l2ef2d0* owned;

    ~UnkRecord_l2ef2d0() {
        UnkOwnedObj_l2ef2d0* old = owned;
        owned = nullptr;
        if (old)
            old->vf_0x10();
    }
};
static_assert(__builtin_offsetof(UnkRecord_l2ef2d0, owned) == 0x30, "UnkRecord_l2ef2d0 layout");

// A record owned through a pointer; moving leaves the source empty.
struct UnkRecordPtr_l2ef2d0 {
    UnkRecord_l2ef2d0* ptr;

    UnkRecord_l2ef2d0* Release() {
        UnkRecord_l2ef2d0* p = ptr;
        ptr = nullptr;
        return p;
    }
    UnkRecordPtr_l2ef2d0(UnkRecordPtr_l2ef2d0&& other) : ptr(other.Release()) {}
    ~UnkRecordPtr_l2ef2d0() {
        UnkRecord_l2ef2d0* old = ptr;
        ptr = nullptr;
        delete old;
    }
};

struct UnkRecordArrays_l2ef2d0 {
    UnkRecordPtr_l2ef2d0* records;
    CStrId* names;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::IAllocator* allocator;
};

// Grows both arrays to `new_capacity`: the records and the names are moved.
void fn_71002ef2d0(UnkRecordArrays_l2ef2d0* self, u32 new_capacity) {
    if (self->capacity >= new_capacity)
        return;
    UnkRecordPtr_l2ef2d0* records = static_cast<UnkRecordPtr_l2ef2d0*>(
        self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkRecordPtr_l2ef2d0), 8, self->flags));
    CStrId* names = static_cast<CStrId*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(CStrId), 8, self->flags));
    if (UnkRecordPtr_l2ef2d0* old = self->records) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            new (&records[j]) UnkRecordPtr_l2ef2d0(static_cast<UnkRecordPtr_l2ef2d0&&>(*old));
            old->~UnkRecordPtr_l2ef2d0();
        }
        self->allocator->vf_0x48(self->records);
    }
    if (CStrId* old = self->names) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            CStrId::MoveConstructReloading(&names[j], *old);
            old->Reset();
        }
        self->allocator->vf_0x48(self->names);
    }
    self->records = records;
    self->names = names;
    self->capacity = new_capacity;
}
