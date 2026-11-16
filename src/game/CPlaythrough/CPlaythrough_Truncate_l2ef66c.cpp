// Unit 424 (CPlaythrough's neighbours): shrinks an array of records (a counted reference and an array
// of named reflected values each) to its first n, destroying the rest. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

// Two string ids and a reflected value.
struct UnkNamedValue_l2ef66c {
    CStrId first;
    CStrId second;
    u64 field_0x10;
    UnkReflValue value;
    u64 field_0x30;

    ~UnkNamedValue_l2ef66c();
};
static_assert(sizeof(UnkNamedValue_l2ef66c) == 0x38, "UnkNamedValue_l2ef66c layout");

inline UnkNamedValue_l2ef66c::~UnkNamedValue_l2ef66c() {
    fn_71000a1c60(&value);
}

using UnkNamedValues_l2ef66c = base::global::CRntVector<UnkNamedValue_l2ef66c>;

// A reference-counted object, destroyed through its vtable (slot 0x10) when the last reference goes.
struct UnkCounted_l2ef66c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 ref_count;
};

struct UnkCountedRef_l2ef66c {
    UnkCounted_l2ef66c* ptr;

    ~UnkCountedRef_l2ef66c() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkRecord_l2ef66c {
    u64 field_0x0;
    UnkCountedRef_l2ef66c ref;
    UnkNamedValues_l2ef66c values;
    u64 field_0x38;

    ~UnkRecord_l2ef66c() { values.Free(); }
};
static_assert(sizeof(UnkRecord_l2ef66c) == 0x40, "UnkRecord_l2ef66c layout");

struct UnkRecords_l2ef66c {
    UnkRecord_l2ef66c* data;
    u32 count;
};

// Destroys the records past `n` and keeps the first n.
void fn_71002ef66c(UnkRecords_l2ef66c* self, u32 n) {
    for (u64 i = n; i < self->count; ++i)
        self->data[i].~UnkRecord_l2ef66c();
    self->count = n;
}
