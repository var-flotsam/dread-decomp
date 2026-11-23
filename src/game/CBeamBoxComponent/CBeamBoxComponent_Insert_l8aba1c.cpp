// CBeamBoxComponent's unit (tools/tu_map.py unit 1355): inserts a copy of an entry (a callback set and a
// string id, 0x48 bytes) at an index of the entry array, moving the later entries up one place after growing
// the array (fn_71008ab4e0); at the end it appends (fn_71008ab6b4). Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/CBeamBoxComponent.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CDemolitionBlockComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkBeamSetBase_l8aba1c {
    char* vtable;
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;

    UnkBeamSetBase_l8aba1c() : vtable(reinterpret_cast<char*>(g_unk_7101a20998) + 0x10), field_0x30(0) {}
    ~UnkBeamSetBase_l8aba1c() { vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10; }
};

// A callback set with the refcounted owner it belongs to; its move constructor is out of line
// (fn_71007b1928).
struct UnkBeamSet_l8aba1c : UnkBeamSetBase_l8aba1c {
    base::UnkRefHandle owner;

    UnkBeamSet_l8aba1c(const UnkBeamSet_l8aba1c& other) {
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        owner.ptr = nullptr;
        owner.Set(other.owner.ptr);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
    }
    ~UnkBeamSet_l8aba1c() {
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        if (base::UnkRefCounted* p = owner.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkBeamId_l8aba1c {
    UnkStringData* data;

    UnkBeamId_l8aba1c(const UnkBeamId_l8aba1c& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkBeamId_l8aba1c(UnkBeamId_l8aba1c&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    ~UnkBeamId_l8aba1c() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkBeamEntry_l8aba1c {
    UnkBeamSet_l8aba1c set;
    UnkBeamId_l8aba1c id;
};
static_assert(sizeof(UnkBeamEntry_l8aba1c) == 0x48, "entry size");

struct UnkBeamVector_l8aba1c {
    UnkBeamEntry_l8aba1c* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_71008aba1c(UnkBeamVector_l8aba1c* self, const UnkBeamEntry_l8aba1c* value, u32 index) {
    if (index == self->count) {
        fn_71008ab6b4(reinterpret_cast<UnkEntryVector*>(self), reinterpret_cast<const UnkEntry*>(value));
        return;
    }
    u32 n = self->count;
    u32 new_capacity = self->capacity;
    if (new_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    fn_71008ab4e0(reinterpret_cast<UnkEntryVector*>(self), new_capacity);
    for (u32 i = self->count - 1; i >= index; --i) {
        UnkBeamEntry_l8aba1c* e = &self->data[i];
        fn_71007b1928(&e[1].set, &e->set);
        new (&e[1].id) UnkBeamId_l8aba1c(static_cast<UnkBeamId_l8aba1c&&>(e->id));
        self->data[i].~UnkBeamEntry_l8aba1c();
        if (i == 0)
            break;
    }
    new (&self->data[index]) UnkBeamEntry_l8aba1c(*value);
    ++self->count;
}
