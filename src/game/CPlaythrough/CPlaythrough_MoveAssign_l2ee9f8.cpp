// Unit 424 (CPlaythrough's neighbours): the move assignment of an iterator value over owned records
// keyed by string ids. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// An object owned through a pointer and destroyed through its vtable (slot 0x10).
struct UnkOwnedObj_l2ee9f8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkRecord_l2ee9f8 {
    u64 field_0x0;
    CStrId name;
    u8 pad_0x10[0x20];
    UnkOwnedObj_l2ee9f8* owned;

    ~UnkRecord_l2ee9f8() {
        UnkOwnedObj_l2ee9f8* old = owned;
        owned = nullptr;
        if (old)
            old->vf_0x10();
    }
};
static_assert(__builtin_offsetof(UnkRecord_l2ee9f8, owned) == 0x30, "UnkRecord_l2ee9f8 layout");

// A record owned through a pointer; moving leaves the source empty.
struct UnkRecordPtr_l2ee9f8 {
    UnkRecord_l2ee9f8* ptr;

    UnkRecord_l2ee9f8* Release() {
        UnkRecord_l2ee9f8* p = ptr;
        ptr = nullptr;
        return p;
    }
    void Reset(UnkRecord_l2ee9f8* p) {
        UnkRecord_l2ee9f8* old = ptr;
        ptr = p;
        if (old)
            delete old;
    }
    UnkRecordPtr_l2ee9f8& operator=(UnkRecordPtr_l2ee9f8&& other) {
        Reset(other.Release());
        return *this;
    }
};

typedef base::UnkIterValue<UnkRecordPtr_l2ee9f8, CStrId> UnkRecordIterValue_l2ee9f8;

// Moves the (id, record) pair `other` holds or refers to into the one `self` holds or refers to.
UnkRecordIterValue_l2ee9f8* fn_71002ee9f8(UnkRecordIterValue_l2ee9f8* self, UnkRecordIterValue_l2ee9f8* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        self->Element() = static_cast<UnkRecordPtr_l2ee9f8&&>(other->value.element);
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        UnkRecordPtr_l2ee9f8& src = *other->ref.element;
        self->Element() = static_cast<UnkRecordPtr_l2ee9f8&&>(src);
    }
    return self;
}
