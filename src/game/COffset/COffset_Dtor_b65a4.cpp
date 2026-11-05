// The destructors of two reflected classes with the same members (tools/tu_map.py: COffset
// fn_710060ef24, CTentacle fn_7100708a38): after the vtable, the string-id array at +0x10 (UnkVector) is
// freed, then the string id at +0x8 released. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringPool;

extern void* g_unk_7101a56150[];  // COffset's vtable
extern void* g_unk_7101a6c020[];  // CTentacle's vtable

// A string id released in two steps (the original reloads `data` and the count after the decrement,
// as CStrId::Reset does), then left empty.
struct UnkReleasedId_b65a4 : base::global::UnkStrIdBase {
    ~UnkReleasedId_b65a4() {
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

template <int Which>
struct UnkNamedIdList_b65a4 {
    void** vtable;
    UnkReleasedId_b65a4 name;                    // 0x8
    base::UnkVector<UnkReleasedId_b65a4> ids;    // 0x10

    __attribute__((always_inline)) ~UnkNamedIdList_b65a4() {
        vtable = reinterpret_cast<void**>(
            reinterpret_cast<char*>(Which == 0 ? g_unk_7101a56150 : g_unk_7101a6c020) + 0x10);
        ids.Free();
    }
};

void fn_710060ef24(UnkNamedIdList_b65a4<0>* self) {
    self->~UnkNamedIdList_b65a4();
}

void fn_7100708a38(UnkNamedIdList_b65a4<1>* self) {
    self->~UnkNamedIdList_b65a4();
}
