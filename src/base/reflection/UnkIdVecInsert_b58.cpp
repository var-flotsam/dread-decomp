// Unit 83 (base::reflection): fn_71000ce49c and fn_71000d0c58 move a string id in at `index` of
// an id vector, shifting the later ids up: base::UnkVector's Insert written out over the out-of-line Reserve. fn_71000ce49c works on
// a CStrId vector (base::UnkStrIdVec, Reserve fn_710000542c), fn_71000d0c58 on a file-path id
// vector (UnkResPathVec, Reserve fn_7100018450). Each shifted id is taken bitwise and its slot
// released in two steps (decrement, then reload and test). Names are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/UnkResPathVec.h"
#include "base/global/CStrId.h"
#include "types.h"

namespace {

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkMovedId_b58i {
    UnkStringData* data;
};

template <class Vec, UnkStringPool** Pool, void (*ReserveFn)(Vec*, u32)>
__attribute__((always_inline)) inline void Insert(Vec* self, UnkMovedId_b58i* value, u32 index) {
    auto** data = reinterpret_cast<UnkStringData***>(&self->data);
    u32 n = self->count;
    u32 cap = self->capacity;
    if (n == index) {
        if (cap <= n && self->step != 0)
            cap = (index / self->step + 1) * self->step;
        if ((s32)cap <= (s32)index)
            for (;;) {
            }
        ReserveFn(self, cap);
        index = self->count;
    } else {
        if (cap <= n && self->step != 0)
            cap = (n / self->step + 1) * self->step;
        ReserveFn(self, cap);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkStringData** q = &(*data)[i];
            q[1] = q[0];
            q[0] = &(*Pool)->empty;
            if (UnkStringPool* pool = *Pool) {
                UnkStringData** slot = &(*data)[i];
                if (*slot && *slot != &pool->empty) {
                    --(*slot)->ref_count;
                    if ((*slot)->ref_count == 0) {
                        fn_71000803a4(pool, *slot);
                        pool = *Pool;
                    }
                }
                *slot = &pool->empty;
            }
            if (i == 0)
                break;
        }
    }
    // Copied bitwise: the original reloads the count after storing the id.
    __builtin_memcpy(&(*data)[index], &value->data, sizeof(UnkStringData*));
    value->data = &(*Pool)->empty;
    ++self->count;
}

}  // namespace

void fn_71000ce49c(base::UnkStrIdVec* self, base::global::CStrId* value, u32 index) {
    Insert<base::UnkStrIdVec, &g_unk_7101d47108, fn_710000542c>(
        self, reinterpret_cast<UnkMovedId_b58i*>(value), index);
}

void fn_71000d0c58(UnkResPathVec* self, base::global::CFilePathStrId* value, u32 index) {
    Insert<UnkResPathVec, &g_unk_7101d47200, fn_7100018450>(
        self, reinterpret_cast<UnkMovedId_b58i*>(value), index);
}

