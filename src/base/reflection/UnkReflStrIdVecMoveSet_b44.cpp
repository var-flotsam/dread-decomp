// A reflected array's "move-assign at index" slot (CCollectionType +0x148) for arrays of string
// ids: the element at the index is fetched out of line (growing the array when it may), released
// in two steps and given the source's id; the source gets the empty id (as CStrId::ReleaseAndTake).
// One for string ids, one for file-path ids. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringPool;

template <UnkStringPool*& Pool>
__attribute__((always_inline)) static inline void ReleaseAndTake(base::UnkStrIdMemberOf<Pool>* self,
                                                                 base::UnkStrIdMemberOf<Pool>* other) {
    if (UnkStringPool* pool = Pool) {
        if (self->data && self->data != &pool->empty) {
            --self->data->ref_count;
            if (self->data->ref_count == 0) {
                fn_71000803a4(pool, self->data);
                pool = Pool;
            }
        }
        self->data = &pool->empty;
    }
    self->data = other->data;
    other->data = &Pool->empty;
}

// fn_710008c174's signature as src/game/CSubAreaManager.cpp declares it.
using UnkPathIdMember = base::UnkStrIdMemberOf<g_unk_7101d47200>;
UnkPathIdMember* fn_710008c174(base::global::CRntVector<UnkPathIdMember>*, u32);
// The element fetch of fn_71000cdd48's array.

void fn_71000cdd48(base::global::CRntVector<base::UnkStrIdMember>* self, const u32* index, base::UnkStrIdMember* value) {
    ReleaseAndTake(fn_71000ce740(self, *index), value);
}

void fn_71000d0504(base::global::CRntVector<UnkPathIdMember>* self, const u32* index, UnkPathIdMember* value) {
    ReleaseAndTake(fn_710008c174(self, *index), value);
}
