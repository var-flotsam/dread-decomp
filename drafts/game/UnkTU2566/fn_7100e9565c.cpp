// Unit 2566 (minimap manager): removes the first string id equal to `id` from an array of ids,
// moving the later ones down. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;


struct UnkIdEntry_le9565c {
    UnkStringData* data;

    UnkIdEntry_le9565c(UnkIdEntry_le9565c&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkIdEntry_le9565c() {
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
    bool operator==(const UnkIdEntry_le9565c& o) const { return data == o.data; }
};


void fn_7100e9565c(base::global::CRntVector<UnkIdEntry_le9565c>* self, const UnkIdEntry_le9565c* id) {
    for (u32 i = 0; i < self->count; ++i) {
        if (self->data[i].data == id->data) {
            self->data[i].~UnkIdEntry_le9565c();
            for (u32 j = i; j < self->count - 1; ++j) {
                new (&self->data[j]) UnkIdEntry_le9565c(static_cast<UnkIdEntry_le9565c&&>(self->data[j + 1]));
                self->data[j + 1].~UnkIdEntry_le9565c();
            }
            self->count = self->count - 1;
            return;
        }
    }
}
