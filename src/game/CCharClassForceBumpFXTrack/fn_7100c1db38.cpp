// Destructors of two classes holding a string id at 0x18 and an array of string ids at 0x20:
// the class's vtable is set, the array's ids are released and the array freed, the id at 0x18
// is released, then the base destructor (fn_710006b5dc) runs. Also two destructors of a key
// with an array of id/word records.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id (or, with the file-path pool, a path id) whose destructor decrements and tests in two
// steps (the original reloads the id after the decrement).
template <UnkStringPool** Pool>
struct UnkReleasedStrIdT {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedStrIdT() {
        if (UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

using UnkReleasedStrId = UnkReleasedStrIdT<&g_unk_7101d47108>;

template <class T>
struct UnkReleasedArray {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkReleasedArray() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~T();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

void fn_710006b5dc(void* self);  // the base's destructor

template <void** Vtable>
struct UnkIdListHolder {
    void** vtable;
    u8 unk_0x8[0x10];
    UnkReleasedStrId id;
    UnkReleasedArray<UnkReleasedStrId> ids;

    __attribute__((always_inline)) ~UnkIdListHolder() { vtable = Vtable + 2; }
};

template <void** Vtable>
__attribute__((always_inline)) static inline void Destroy(UnkIdListHolder<Vtable>* self) {
    self->~UnkIdListHolder();
    fn_710006b5dc(self);
}

extern void* g_unk_7101aa3030[];
extern void* g_unk_7101aa3440[];

void fn_7100c1db38(UnkIdListHolder<g_unk_7101aa3030>* self) { Destroy(self); }
void fn_7100c274e8(UnkIdListHolder<g_unk_7101aa3440>* self) { Destroy(self); }

struct UnkIdWord {
    UnkReleasedStrId id;
    u64 field_0x8;
};

struct UnkIdWordList {
    UnkReleasedStrId key;
    UnkReleasedArray<UnkIdWord> entries;
};

void fn_7100b480a0(UnkIdWordList* self) { self->~UnkIdWordList(); }
void fn_7100ea8cd4(UnkIdWordList* self) { self->~UnkIdWordList(); }

// The same with path ids (the entries' after a word), in another unit.
struct UnkPathIdWord {
    u64 field_0x0;
    UnkReleasedStrIdT<&g_unk_7101d47200> id;
};

struct UnkPathIdWordList {
    UnkReleasedStrIdT<&g_unk_7101d47200> key;
    UnkReleasedArray<UnkPathIdWord> entries;
};

void fn_71011aa9d8(UnkPathIdWordList* self) { self->~UnkPathIdWordList(); }

// The same with a second string id at 0x48 and a word (released first).
template <void** Vtable>
struct UnkIdListIdHolder {
    void** vtable;
    u8 unk_0x8[0x10];
    UnkReleasedStrId id;
    UnkReleasedArray<UnkReleasedStrId> ids;
    UnkReleasedStrId id_0x48;
    u32 field_0x50;

    __attribute__((always_inline)) ~UnkIdListIdHolder() { vtable = Vtable + 2; }
};

template <void** Vtable>
__attribute__((always_inline)) static inline void Destroy(UnkIdListIdHolder<Vtable>* self) {
    self->~UnkIdListIdHolder();
    fn_710006b5dc(self);
}

extern void* g_unk_7101aa2b80[];
extern void* g_unk_7101aa7d00[];

void fn_7100c12a84(UnkIdListIdHolder<g_unk_7101aa2b80>* self) { Destroy(self); }
void fn_7100d21ff8(UnkIdListIdHolder<g_unk_7101aa7d00>* self) { Destroy(self); }
