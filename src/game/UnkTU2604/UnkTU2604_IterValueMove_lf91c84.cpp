// Unit 2604: the move helper of the iterator value over a reflected collection of (string id, record) pairs: the key
// id becomes the empty string, the record's own id is copied (its count raised). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkTU2604MovedId {
    UnkStringData* data;

    UnkTU2604MovedId(UnkTU2604MovedId&& other) {
        data = other.data;
        other.data = &g_unk_7101d47108->empty;
    }
};

struct UnkTU2604Record {
    CStrId id;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;

    UnkTU2604Record(UnkTU2604Record&& other) = default;
};

void fn_7100f91c84(void* dst, const void* src) {
    base::refl_IterValueMove<UnkTU2604Record, UnkTU2604MovedId>(dst, src);
}
