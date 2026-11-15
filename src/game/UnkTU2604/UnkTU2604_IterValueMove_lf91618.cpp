// Unit 2604: the move helper of the iterator value over a reflected array of 0x28-byte elements that begin with
// a string id as key and a record that begins with a string id (both become the empty string). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkTU2604MovedId {
    UnkStringData* data;

    UnkTU2604MovedId(UnkTU2604MovedId&& other) {
        data = other.data;
        other.data = &g_unk_7101d47108->empty;
    }
};

struct UnkTU2604Elem {
    UnkTU2604MovedId id;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;

    UnkTU2604Elem(UnkTU2604Elem&& other) = default;
};

void fn_7100f91618(void* dst, const void* src) {
    base::refl_IterValueMove<UnkTU2604Elem, UnkTU2604MovedId>(dst, src);
}
