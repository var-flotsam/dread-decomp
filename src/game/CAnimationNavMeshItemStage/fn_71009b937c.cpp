// build-flags: no-strict-aliasing
// CAnimationNavMeshItemStage unit: fn_71009b937c copies one fixed-capacity list of (string id, flag)
// entries over another: the entries both hold are assigned, the other's extra ones appended, and the
// count taken. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkStrIdFlagEntry_l9b937c {
    CStrId id;
    u8 flag;

    UnkStrIdFlagEntry_l9b937c(const UnkStrIdFlagEntry_l9b937c& other) : id(other.id), flag(other.flag) {}
    UnkStrIdFlagEntry_l9b937c& operator=(const UnkStrIdFlagEntry_l9b937c& other) {
        id = other.id;
        flag = other.flag;
        return *this;
    }
};

// A count followed by inline storage for the entries (packed: the entries start at +4).
struct __attribute__((packed)) UnkStrIdFlagList_l9b937c {
    u32 count;
    u8 storage[16 * sizeof(UnkStrIdFlagEntry_l9b937c)];

    UnkStrIdFlagEntry_l9b937c* items() { return reinterpret_cast<UnkStrIdFlagEntry_l9b937c*>(storage); }
    const UnkStrIdFlagEntry_l9b937c* items() const {
        return reinterpret_cast<const UnkStrIdFlagEntry_l9b937c*>(storage);
    }
};

UnkStrIdFlagList_l9b937c* fn_71009b937c(UnkStrIdFlagList_l9b937c* self, const UnkStrIdFlagList_l9b937c* other) {
    if (self == other)
        return self;
    if (self->count > other->count) {
        for (u32 i = 0; i < other->count; ++i)
            self->items()[i] = other->items()[i];
    } else {
        for (u32 i = 0; i < self->count; ++i)
            self->items()[i] = other->items()[i];
    }
    for (u32 i = self->count; i < other->count; ++i) {
        new (&self->items()[self->count]) UnkStrIdFlagEntry_l9b937c(other->items()[i]);
        ++self->count;
    }
    self->count = other->count;
    return self;
}
