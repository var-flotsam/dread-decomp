// SDropProbabilities' unit (tools/tu_map.py): the move assignment of a drop record: plain values up to
// +0x5c, an id at +0x60 moved (released, then taken from the other, which gets the empty id), six
// words at +0x68 and an 11-byte tail at +0x80. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkDropRecordHead_b46a2 {
    f32 values[0x17];
};

struct UnkDropRecordTail_b46a2 {
    u8 bytes[0xb];
};

struct UnkDropRecord_b46a2 {
    UnkDropRecordHead_b46a2 head;
    u8 pad_0x5c[4];
    CStrId id;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
    u32 field_0x7c;
    UnkDropRecordTail_b46a2 tail;
};

UnkDropRecord_b46a2* fn_71007c82dc(UnkDropRecord_b46a2* self, UnkDropRecord_b46a2* other) {
    if (self != other) {
        self->head = other->head;
        self->id.ReleaseAndTake(other->id);
        self->field_0x68 = other->field_0x68;
        self->field_0x6c = other->field_0x6c;
        self->field_0x70 = other->field_0x70;
        self->field_0x74 = other->field_0x74;
        self->field_0x78 = other->field_0x78;
        self->field_0x7c = other->field_0x7c;
        self->tail = other->tail;
    }
    return self;
}
