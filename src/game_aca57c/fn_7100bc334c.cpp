// Destructors of two classes laid out like the records copied in fn_7100bc3500: the class's
// vtable is set, the id at 0x18 is handed to fn_7100027044, then the refcounted pointer at 0x28
// (count at 0x54, freed by fn_710001737c), the file path at 0x20 and the string ids at 0x18 and
// 0x10 are released and the base destructor (fn_7100067ed4) runs.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id (of the pool `Pool` points to) whose destructor decrements and tests in two steps
// (the original reloads the id after the decrement).
template <UnkStringPool** Pool>
struct UnkReleasedId {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedId() {
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
using UnkReleasedStrId = UnkReleasedId<&g_unk_7101d47108>;
using UnkReleasedPathId = UnkReleasedId<&g_unk_7101d47200>;

struct UnkShared54Ptr {
    UnkCountedResource* ptr;

    __attribute__((always_inline)) ~UnkShared54Ptr() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }
};

void fn_7100027044(UnkReleasedStrId* id);
void fn_7100067ed4(void* self);  // the base's destructor

template <char* Vtable>
struct UnkPathRefBase {
    char* vtable;
    u8 unk_0x8[0x8];
    UnkReleasedStrId a;
    UnkReleasedStrId b;
    UnkReleasedPathId path;
    UnkShared54Ptr shared;

    __attribute__((always_inline)) ~UnkPathRefBase() {
        vtable = Vtable + 0x10;
        fn_7100027044(&b);
    }
};

template <char* Vtable>
__attribute__((always_inline)) static inline void Destroy(UnkPathRefBase<Vtable>* self) {
    self->~UnkPathRefBase();
    fn_7100067ed4(self);
}

extern char unk_7101aa15b8[];
extern char unk_7101aa1ea8[];

void fn_7100bc334c(UnkPathRefBase<unk_7101aa15b8>* self) { Destroy(self); }
void fn_7100be4478(UnkPathRefBase<unk_7101aa1ea8>* self) { Destroy(self); }
