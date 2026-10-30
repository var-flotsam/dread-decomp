// Move assignment of an indexed entry (its array takes over the other's and the flag at 0x30 is
// copied), on its own and on the entry stored under an index (fn_7100c0a828 finds or makes it).
#include "base/UnkVector.h"
#include "types.h"

struct UnkElem58 {
    virtual void vf_0x0();
    virtual ~UnkElem58();
    u8 unk_0x8[0x50];
};

struct UnkIndexedEntry {
    u64 field_0x0;
    base::UnkVector<UnkElem58> items;
    bool field_0x30;

    UnkIndexedEntry& operator=(UnkIndexedEntry&& other) {
        items = static_cast<base::UnkVector<UnkElem58>&&>(other.items);
        field_0x30 = other.field_0x30;
        return *this;
    }
};

struct UnkIndexedTable;
UnkIndexedEntry* fn_7100c0a828(UnkIndexedTable* self, u32 index);

void fn_7100c0a1d4(UnkIndexedTable* self, const u32* index, UnkIndexedEntry&& value) {
    *fn_7100c0a828(self, *index) = static_cast<UnkIndexedEntry&&>(value);
}

// A move assignment of the same layout that skips the flag too when assigning to itself.
UnkIndexedEntry* fn_7100c07b70(UnkIndexedEntry* self, UnkIndexedEntry&& other) {
    if (self != &other)
        *self = static_cast<UnkIndexedEntry&&>(other);
    return self;
}

// The same for entries whose array holds 0x20-byte polymorphic elements: fn_710105ef34 (stored under
// an index, fn_710105f810 finds or makes it) and fn_710105cfe8.
struct UnkElem20 {
    virtual void vf_0x0();
    virtual ~UnkElem20();
    u8 unk_0x8[0x18];
};

struct UnkIndexedEntry20 {
    u64 field_0x0;
    base::UnkVector<UnkElem20> items;
    bool field_0x30;

    UnkIndexedEntry20& operator=(UnkIndexedEntry20&& other) {
        items = static_cast<base::UnkVector<UnkElem20>&&>(other.items);
        field_0x30 = other.field_0x30;
        return *this;
    }
};

struct UnkIndexedTable20;
UnkIndexedEntry20* fn_710105f810(UnkIndexedTable20* self, u32 index);

void fn_710105ef34(UnkIndexedTable20* self, const u32* index, UnkIndexedEntry20&& value) {
    *fn_710105f810(self, *index) = static_cast<UnkIndexedEntry20&&>(value);
}

UnkIndexedEntry20* fn_710105cfe8(UnkIndexedEntry20* self, UnkIndexedEntry20&& other) {
    if (self != &other)
        *self = static_cast<UnkIndexedEntry20&&>(other);
    return self;
}
