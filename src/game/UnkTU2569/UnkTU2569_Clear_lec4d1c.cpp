// Unit 2569: resets a record to its default: the kind to 6, both string ids to "", and its array of
// 32-byte entries emptied and freed; then both ids are released. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2569.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdRecord_lec4d1c {
    u32 kind;
    base::global::UnkStrId field_0x8;
    base::global::UnkStrId field_0x10;
    UnkIdVec_ovb0245 entries;  // 0x18
};

// Releases an id and leaves it empty, the count decremented, then tested (as CStrId::Reset).
static inline void Release(base::global::UnkStrIdBase* id) {
    using namespace base::global;
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (id->data && id->data != &pool->empty) {
            --id->data->ref_count;
            if (id->data->ref_count == 0) {
                StringPool_Release(pool, id->data);
                pool = g_unk_7101d47108;
            }
        }
        id->data = &pool->empty;
    }
}

void fn_7100ec4d1c(UnkIdRecord_lec4d1c* self) {
    self->kind = 6;
    {
        base::global::UnkStrId empty(g_unk_710162e2ed, true);
        self->field_0x8 = static_cast<base::global::UnkStrId&&>(empty);
    }
    {
        base::global::UnkStrId empty(g_unk_710162e2ed, true);
        self->field_0x10 = static_cast<base::global::UnkStrId&&>(empty);
    }
    fn_7100ec4fd4(&self->entries, 0);
    if (self->entries.data) {
        fn_7100ec4fd4(&self->entries, 0);
        self->entries.allocator->vf_0x48(self->entries.data);
        self->entries.capacity = 0;
    }
    Release(&self->field_0x10);
    Release(&self->field_0x8);
}
