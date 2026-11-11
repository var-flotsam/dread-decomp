#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkRcObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        u32 field_0x8;
    };
};

struct UnkRcRef {
    UnkRcObject* ptr;

    UnkRcRef(const UnkRcRef& other) : ptr(nullptr) {
        UnkRcObject* o = other.ptr;
        if (o) {
            ++o->field_0x8;
            Release();
            ptr = o;
        }
    }
    ~UnkRcRef() { Release(); }

    void Release() {
        UnkRcObject* o = ptr;
        if (o && --o->field_0x8 == 0)
            o->vf_0x10();
    }
};

using UnkRcRefVector = base::UnkVector<UnkRcRef>;

// UnkVector::EraseAt as fn_71004bf588 writes it, over reference-counted pointers.
static inline void EraseAt(UnkRcRefVector* self, u32 index) {
    if (index < self->count) {
        u64 i = index;
        self->data[i].~UnkRcRef();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkRcRef* cur = &self->data[i];
            new (cur) UnkRcRef(cur[1]);
            (&self->data[i])[1].~UnkRcRef();
        }
        self->count = self->count - 1;
    }
}

void fn_7100241440(UnkRcRefVector* self, const u32* it) {
    UnkRcRef* base = reinterpret_cast<UnkRcRef*>(self);
    u64 offset = (u64)*it * sizeof(UnkRcRef);
    UnkRcRef* p = reinterpret_cast<UnkRcRef*>(reinterpret_cast<char*>(base) + offset);
    EraseAt(self, (u32)(p - base));
}
