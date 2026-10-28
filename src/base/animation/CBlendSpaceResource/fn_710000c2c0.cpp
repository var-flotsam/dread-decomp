// Copies of fn_7100d3d424 (src/game/logic/collision/CCollider/fn_7100d3d424.cpp) for a variant holding a file-path id and another
// reference, at other offsets, naming their own callees and data (the comments are adapted from the original's):
// fn_710000c2c0, fn_710000c75c (at 0x20), fn_710000d728, fn_710000dd0c (at 0x10), fn_7100266b28, fn_7100266fc4 (at
// 0x18), fn_710026735c, fn_7100267954 (at 0x8). Reset of a two-state variant (state 1 holds a string id and a
// reference; state 2 holds nothing to destroy) to state 0.
#include "base/UnkCountedResource.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id of CFilePathStrId's pool whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkReleasedFilePathId {
    UnkStringData* data;

    ~UnkReleasedFilePathId() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// A reference-counted resource (base/UnkCountedResource.h), dropped when the pointer dies.
struct UnkCountedPtr {
    UnkCountedResource* ptr;

    ~UnkCountedPtr() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }
};

struct UnkIdRef {
    UnkReleasedFilePathId id;
    UnkCountedPtr ref;
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

template <u32 Offset>
struct UnkVariantHolder {
    u8 unk_0x0[Offset];
    UnkIdRefVariant variant;
};

void fn_710000c2c0(UnkVariantHolder<0x20>* self) { self->variant.Reset(); }
void fn_710000c75c(UnkVariantHolder<0x20>* self) { self->variant.Reset(); }
void fn_710000d728(UnkVariantHolder<0x10>* self) { self->variant.Reset(); }
void fn_710000dd0c(UnkVariantHolder<0x10>* self) { self->variant.Reset(); }
void fn_7100266b28(UnkVariantHolder<0x18>* self) { self->variant.Reset(); }
void fn_7100266fc4(UnkVariantHolder<0x18>* self) { self->variant.Reset(); }
void fn_710026735c(UnkVariantHolder<0x8>* self) { self->variant.Reset(); }
void fn_7100267954(UnkVariantHolder<0x8>* self) { self->variant.Reset(); }
