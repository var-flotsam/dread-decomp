// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): appends a default record
// (a pair of empty string ids and 8 bytes of data) to an engine array and returns the last element.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkIdPair_7100a8e07c;
void fn_7100ef1970(UnkIdPair_7100a8e07c* self);

// A string id that moves by taking the id and leaving the pool's empty string.
struct UnkIdSlot_eb498 {
    UnkStringData* data;

    UnkIdSlot_eb498() = default;
    UnkIdSlot_eb498(UnkIdSlot_eb498&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    ~UnkIdSlot_eb498() {
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

// Both ids start as the empty string (fn_7100ef1970).
struct UnkIdPair_eb498 {
    UnkIdSlot_eb498 first;
    UnkIdSlot_eb498 second;

    UnkIdPair_eb498() { fn_7100ef1970(reinterpret_cast<UnkIdPair_7100a8e07c*>(this)); }
    UnkIdPair_eb498(UnkIdPair_eb498&& other) = default;
};

struct UnkRecord_eb498 {
    UnkIdPair_eb498 ids;
    u32 field_0x10;
    u32 field_0x14;
};
static_assert(sizeof(UnkRecord_eb498) == 0x18, "UnkRecord_eb498 layout");

UnkRecord_eb498* fn_7100eeb498(base::global::CRntVector<UnkRecord_eb498>* self) {
    u32 n = self->count + 1;
    self->Reserve(n);
    for (u32 i = self->count; i < n; ++i)
        new (&self->data[i]) UnkRecord_eb498();
    self->count = n;
    UnkRecord_eb498* end = &self->data[n];
    return n ? &end[-1] : end;
}
