// Unit 424 (CPlaythrough's neighbours): erases an owned record from an array with a string id per
// record alongside, by moving the last record and id into its place. Returns the erased record's
// place. Names and layouts are placeholders.
#include <memory>
#include <new>

#include "base/UnkCloneCalls.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// An object owned through a pointer and destroyed through its vtable (slot 0x10).
struct UnkOwnedObj_l2ec674 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkRecord_l2ec674 {
    u64 field_0x0;
    CStrId name;
    u8 pad_0x10[0x20];
    UnkOwnedObj_l2ec674* owned;

    ~UnkRecord_l2ec674() {
        UnkOwnedObj_l2ec674* old = owned;
        owned = nullptr;
        if (old)
            old->vf_0x10();
    }
};
static_assert(__builtin_offsetof(UnkRecord_l2ec674, owned) == 0x30, "UnkRecord_l2ec674 layout");

using UnkRecordPtr_l2ec674 = std::unique_ptr<UnkRecord_l2ec674>;

struct UnkRecordArrays_l2ec674 {
    UnkRecordPtr_l2ec674* records;
    CStrId* names;
    u32 count;
};

// The array's types as its caller (fn_71002eeef8) declares them.
struct UnkKeyedValues_71002eef6c;
using UnkValuesByKey = base::UnkKeyedArray<UnkKeyedValues_71002eef6c, u64, u64>;

u64* fn_71002ec674(UnkValuesByKey* a, u64* value) {
    UnkRecordArrays_l2ec674* self = reinterpret_cast<UnkRecordArrays_l2ec674*>(a);
    u32 index = static_cast<u32>(reinterpret_cast<UnkRecordPtr_l2ec674*>(value) - self->records);
    self->records[index].~unique_ptr();
    self->names[index].Reset();
    if (self->count - 1 != index) {
        new (&self->records[index])
            UnkRecordPtr_l2ec674(static_cast<UnkRecordPtr_l2ec674&&>(self->records[self->count - 1]));
        self->records[self->count - 1].~unique_ptr();
        CStrId::MoveConstructReloading(&self->names[index], self->names[self->count - 1]);
        self->names[self->count - 1].Reset();
    }
    --self->count;
    return reinterpret_cast<u64*>(&self->records[index]);
}
