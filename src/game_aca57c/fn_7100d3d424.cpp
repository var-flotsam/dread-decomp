// Resets and assignments of small holders of string ids: a two-state variant (state 1 holds a
// string id and a reference; state 2 holds nothing to destroy) reset to state 0, a counted
// array of ids stored after its count, and a move-assignment of an id with two values that
// destroys the old id first.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkReleasedStrId {
    UnkStringData* data;

    UnkReleasedStrId(UnkReleasedStrId&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    ~UnkReleasedStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkRefPtr {
    UnkRefObject* ptr;

    ~UnkRefPtr() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkIdRef {
    UnkReleasedStrId id;
    UnkRefPtr ref;
};

struct UnkIdRefVariant {
    union {
        UnkIdRef id_ref;
    };
    s32 state;

    void Reset() {
        switch (state) {
        case 1:
            id_ref.~UnkIdRef();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }
};

struct UnkVariantHolder {
    u64 field_0x0;
    UnkIdRefVariant variant;
};

void fn_7100d3d424(UnkVariantHolder* self) { self->variant.Reset(); }
void fn_7100d3d8ac(UnkVariantHolder* self) { self->variant.Reset(); }
void fn_7100d40ed8(UnkIdRefVariant* self) { self->Reset(); }
void fn_7100d414b0(UnkIdRefVariant* self) { self->Reset(); }

// A count followed directly (at +4, packed) by the ids.
#pragma pack(push, 4)
struct UnkCountedIds {
    u32 count;
    UnkReleasedStrId ids[1];
};
#pragma pack(pop)

void fn_7100de4928(UnkCountedIds* self) {
    for (u64 i = 0; i < self->count; ++i)
        self->ids[i].~UnkReleasedStrId();
    self->count = 0;
}

struct UnkIdValues {
    UnkReleasedStrId id;
    u32 field_0x8;
    u32 field_0xc;

    UnkIdValues(UnkIdValues&& other)
        : id(static_cast<UnkReleasedStrId&&>(other.id)), field_0x8(other.field_0x8), field_0xc(other.field_0xc) {}
};

UnkIdValues* fn_7100ead030(UnkIdValues* self, UnkIdValues* other) {
    if (self != other) {
        self->~UnkIdValues();
        new (self) UnkIdValues(static_cast<UnkIdValues&&>(*other));
    }
    return self;
}

struct UnkIdValues4 {
    UnkReleasedStrId id;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;

    UnkIdValues4(UnkIdValues4&& other)
        : id(static_cast<UnkReleasedStrId&&>(other.id)), field_0x8(other.field_0x8), field_0xc(other.field_0xc),
          field_0x10(other.field_0x10), field_0x14(other.field_0x14) {}
};

UnkIdValues4* fn_7100e6e704(UnkIdValues4* self, UnkIdValues4* other) {
    if (self != other) {
        self->~UnkIdValues4();
        new (self) UnkIdValues4(static_cast<UnkIdValues4&&>(*other));
    }
    return self;
}

// Copy-assignment of a polymorphic record: a value, a string id and 0x1c plain bytes (copied as
// one block).
struct UnkIdBlockRecord {
    void* vtable;
    u32 field_0x8;
    CStrId id;
    u8 data[0x1c];

    UnkIdBlockRecord& operator=(const UnkIdBlockRecord& other) {
        if (this != &other) {
            field_0x8 = other.field_0x8;
            id = other.id;
            __builtin_memcpy(data, other.data, sizeof(data));
        }
        return *this;
    }
};

UnkIdBlockRecord* fn_7100cd4af8(UnkIdBlockRecord* self, const UnkIdBlockRecord* other) { return &(*self = *other); }
UnkIdBlockRecord* fn_7100cd4bac(UnkIdBlockRecord* self, const UnkIdBlockRecord* other) { return &(*self = *other); }

// Clears (destroying the elements, keeping the storage) of arrays of ids (at 0xd642f0 of an
// owner) and of ids with two values.
template <class T>
struct UnkClearedArray {
    T* data;
    u32 count;

    void Clear() {
        for (u64 i = 0; i < count; ++i)
            data[i].~T();
        count = 0;
    }
};

struct UnkIdArrayOwner {
    u8 unk_0x0[0xd642f0];
    UnkClearedArray<UnkReleasedStrId> ids;
};

void fn_7100e9b5d0(UnkIdArrayOwner* self) { self->ids.Clear(); }
void fn_7100eaf4e0(UnkClearedArray<UnkIdValues>* self) { self->Clear(); }

struct UnkIdPairValue {
    UnkReleasedStrId first;
    UnkReleasedStrId second;
    u32 field_0x10;
};

// Releases the old id, then takes the other's (leaving it empty).
__attribute__((always_inline)) static inline void MoveAssign(UnkReleasedStrId* self, UnkReleasedStrId* other) {
    self->~UnkReleasedStrId();
    self->data = other->data;
    other->data = &g_unk_7101d47108->empty;
}

UnkIdPairValue* fn_7100c8f970(UnkIdPairValue* self, UnkIdPairValue* other) {
    if (self != other) {
        MoveAssign(&self->first, &other->first);
        MoveAssign(&self->second, &other->second);
        self->field_0x10 = other->field_0x10;
    }
    return self;
}
