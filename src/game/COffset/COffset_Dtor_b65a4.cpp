// The destructors of two reflected classes with the same members (tools/tu_map.py: COffset
// fn_710060ef24, CTentacle fn_7100708a38): after the vtable, the string-id array at +0x10 (CRntVector) is
// freed, then the string id at +0x8 released. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringPool;

extern void* g_unk_7101a56150[];  // COffset's vtable
extern void* g_unk_7101a6c020[];  // CTentacle's vtable

template <int Which>
struct UnkNamedIdList_b65a4 {
    void** vtable;
    base::global::CStrId name;                    // 0x8
    base::global::CRntVector<base::global::CStrId> ids;    // 0x10

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
