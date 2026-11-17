// Clear (eight copies over six value types) and two destructors of hash maps keyed by string
// ids: the clear destroys every node on the map's node list (the value at +8 with its own
// destructor, then the key), frees it through the allocator, then empties the buckets and
// resets the count and the list; the destructors clear the map and destroy its storage.
#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A value of Size bytes, destroyed by Destroy.
template <u32 Size, void (*Destroy)(void*)>
struct UnkMapValue {
    u8 bytes[Size];

    ~UnkMapValue() { Destroy(bytes); }
};

// The node's list link is at Next.
template <u32 Next, void (*Destroy)(void*)>
using UnkIdMap = base::global::CRntDictionary<CStrId, UnkMapValue<Next - 8, Destroy>>;

#define DEFINE_CLEAR(addr, next, destroy)                                \
    void fn_##destroy(void* value);                                      \
    void fn_##addr(UnkIdMap<next, fn_##destroy>* self) { self->Clear(); }

DEFINE_CLEAR(7100f79248, 0x3b0, 7100d8f9e4)
DEFINE_CLEAR(7100f9c184, 0x3b0, 7100d8f9e4)
DEFINE_CLEAR(7100f86f20, 0x70, 7100d90304)
DEFINE_CLEAR(7100f949a8, 0x80, 7100f8da4c)
DEFINE_CLEAR(7100f96df4, 0x80, 7100f8da4c)
DEFINE_CLEAR(71010045d4, 0x58, 7100fed6cc)
DEFINE_CLEAR(71010057a4, 0x58, 7100fee034)
DEFINE_CLEAR(71010422c4, 0x5d8, 7101036e34)

// Two maps' storage, whose destructors free the buckets.
struct UnkIdMapStorage_80 : UnkIdMap<0x80, fn_7100f8da4c> {
    ~UnkIdMapStorage_80();  // fn_7100d90040
};
struct UnkIdMapStorage_3b0 : UnkIdMap<0x3b0, fn_7100d8f9e4> {
    ~UnkIdMapStorage_3b0();  // fn_7100d8f8f8
};

// A map destroyed through its storage after clearing.
template <class Storage>
struct UnkOwnedIdMap : Storage {
    __attribute__((always_inline)) ~UnkOwnedIdMap() { this->Clear(); }
};

void fn_7100f963c8(UnkOwnedIdMap<UnkIdMapStorage_80>* self) {
    self->~UnkOwnedIdMap();
}

void fn_7100f9c84c(UnkOwnedIdMap<UnkIdMapStorage_3b0>* self) {
    self->~UnkOwnedIdMap();
}

