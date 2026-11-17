// game::logic::collision::CCollider's unit (tools/tu_map.py): copies a value that holds either its own
// pair (a word and an array of 8-byte entries; kind 1) or pointers to one held elsewhere (kind 2);
// the copy always owns its pair. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

struct UnkColliderPair_ld434fc {
    u32 field_0x0;
    CRntVector<u64> entries;  // 0x8

    UnkColliderPair_ld434fc(const UnkColliderPair_ld434fc& other) = default;
    UnkColliderPair_ld434fc(const u32& field, const CRntVector<u64>& list) : field_0x0(field), entries(list) {}
};

// A pair held elsewhere.
struct UnkColliderPairRef_ld434fc {
    const u32* field_0x0;
    const CRntVector<u64>* entries;
};

struct UnkColliderValue_ld434fc {
    union {
        UnkColliderPair_ld434fc owned;    // kind 1
        UnkColliderPairRef_ld434fc ref;   // kind 2
    };
    u32 kind;  // 0x30
};
static_assert(__builtin_offsetof(UnkColliderValue_ld434fc, kind) == 0x30, "layout");

void fn_7100d434fc(UnkColliderValue_ld434fc* self, const UnkColliderValue_ld434fc* other) {
    self->kind = 0;
    switch (other->kind) {
    case 1:
        self->kind = 1;
        new (&self->owned) UnkColliderPair_ld434fc(other->owned);
        break;
    case 2:
        self->kind = 1;
        new (&self->owned) UnkColliderPair_ld434fc(*other->ref.field_0x0, *other->ref.entries);
        break;
    }
}
