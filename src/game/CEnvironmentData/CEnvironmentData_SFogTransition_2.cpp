// Copy constructors and assignments of a 0x24-byte record in CEnvironmentData::SFogTransition's unit
// (two identical copies of each): a word, then 28 bytes copied as raw bytes. Names and layouts are
// placeholders.
#include "types.h"

struct UnkFogRecord_0x24 {
    u64 field_0x0;
    u8 payload[28];
};

static inline UnkFogRecord_0x24* CopyRecord(UnkFogRecord_0x24* self, const UnkFogRecord_0x24* other) {
    self->field_0x0 = other->field_0x0;
    __builtin_memcpy(self->payload, other->payload, sizeof(self->payload));
    return self;
}

static inline UnkFogRecord_0x24* AssignRecord(UnkFogRecord_0x24* self, const UnkFogRecord_0x24* other) {
    if (self != other)
        CopyRecord(self, other);
    return self;
}

UnkFogRecord_0x24* fn_7100f607dc(UnkFogRecord_0x24* self, const UnkFogRecord_0x24* other) { return CopyRecord(self, other); }
UnkFogRecord_0x24* fn_7100f60800(UnkFogRecord_0x24* self, const UnkFogRecord_0x24* other) { return CopyRecord(self, other); }
UnkFogRecord_0x24* fn_7100f60828(UnkFogRecord_0x24* self, const UnkFogRecord_0x24* other) { return AssignRecord(self, other); }
UnkFogRecord_0x24* fn_7100f60854(UnkFogRecord_0x24* self, const UnkFogRecord_0x24* other) { return AssignRecord(self, other); }
