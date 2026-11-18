// Unit 2566 (minimap manager): removes the first string id equal to `id` from an array of ids,
// moving the later ones down. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

// Releases the id at `index` and moves the later ones down.
static inline void EraseId(CRntVector<CStrId>& ids, u32 index) {
    ids.data[index].~CStrId();
    for (u32 i = index; i < ids.count - 1; ++i) {
        CStrId::MoveConstructReloading(ids.data + i, *(ids.data + i + 1));
        (ids.data + i + 1)->~CStrId();
    }
    ids.count = ids.count - 1;
}

void fn_7100e9565c(CRntVector<CStrId>* self, const CStrId* id) {
    for (u32 i = 0; i < self->count; ++i) {
        if (self->data[i] == *id) {
            EraseId(*self, i);
            return;
        }
    }
}
