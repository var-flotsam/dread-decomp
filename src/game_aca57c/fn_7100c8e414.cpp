// Move constructor of a two-state holder: state 1 owns a record in place, state 2 refers to
// one through two pointers (its first word and the rest). Either way the record is moved in
// and the holder ends in state 1; string ids are stolen and their sources left empty.
#include <new>
#include "types.h"
#include "base/global/CStrId.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id stolen by a move. The pointer is held as an integer: the original keeps the pool
// it loaded across the stores to the moved-from ids, which a pointer-typed store would forbid.
template <UnkStringPool** Pool>
struct UnkMovedStrId {
    u64 data;

    UnkMovedStrId(UnkMovedStrId&& other) : data(other.data) { other.data = (u64)&(*Pool)->empty; }
};
using UnkMovedId = UnkMovedStrId<&g_unk_7101d47108>;
using UnkMovedPath = UnkMovedStrId<&g_unk_7101d47200>;

// A buffer whose move takes the pointers and count and clears them in the source.
struct UnkMovedBuffer {
    void* begin;
    void* end;
    u32 count;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    void* field_0x18;

    UnkMovedBuffer(UnkMovedBuffer&& other)
        : begin(other.begin), end(other.end), count(other.count), field_0xc(other.field_0xc),
          field_0x10(other.field_0x10), field_0x14(other.field_0x14), field_0x18(other.field_0x18) {
        other.count = 0;
        other.begin = nullptr;
        other.end = nullptr;
    }
};

struct UnkRecordRest {
    UnkMovedId field_0x0;
    UnkMovedPath field_0x8;
    UnkMovedId field_0x10;
    UnkMovedId field_0x18;
    u16 field_0x20;
    u32 field_0x24;
    UnkMovedBuffer field_0x28;
    void* field_0x50;
    UnkMovedId field_0x58;
    UnkMovedId field_0x60;
    u32 field_0x68;
    UnkMovedBuffer field_0x70;
    UnkMovedPath field_0x98;

    UnkRecordRest(UnkRecordRest&& o)
        : field_0x0(static_cast<UnkMovedId&&>(o.field_0x0)),
          field_0x8(static_cast<UnkMovedPath&&>(o.field_0x8)),
          field_0x10(static_cast<UnkMovedId&&>(o.field_0x10)),
          field_0x18(static_cast<UnkMovedId&&>(o.field_0x18)), field_0x20(o.field_0x20),
          field_0x24(o.field_0x24), field_0x28(static_cast<UnkMovedBuffer&&>(o.field_0x28)),
          field_0x50(o.field_0x50), field_0x58(static_cast<UnkMovedId&&>(o.field_0x58)),
          field_0x60(static_cast<UnkMovedId&&>(o.field_0x60)), field_0x68(o.field_0x68),
          field_0x70(static_cast<UnkMovedBuffer&&>(o.field_0x70)),
          field_0x98(static_cast<UnkMovedPath&&>(o.field_0x98)) {}
};

struct UnkRecordRef {
    u32* first;
    UnkRecordRest* rest;
};

struct UnkRecord {
    u32 first;
    UnkRecordRest rest;
};

struct UnkHolder_7100c8e414 {
    union {
        UnkRecord record;
        UnkRecordRef ref;
    };
    s32 state;
};

// NON_MATCHING(minor): in state 2 the original reloads the first pool once more, after moving
// the file path id; the code is otherwise identical.
void fn_7100c8e414(UnkHolder_7100c8e414* self, UnkHolder_7100c8e414* other) {
    self->state = 0;
    switch (other->state) {
    case 1:
        self->state = 1;
        new (&self->record) UnkRecord(static_cast<UnkRecord&&>(other->record));
        break;
    case 2:
        self->state = 1;
        UnkRecordRef ref = other->ref;
        self->record.first = *ref.first;
        new (&self->record.rest) UnkRecordRest(static_cast<UnkRecordRest&&>(*ref.rest));
        break;
    }
}
