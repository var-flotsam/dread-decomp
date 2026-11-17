// From unit 2452 (CCharClassModifyColliderStateBaseTrack): the track's copy constructor: the base
// track's two words, then the six groups of string ids and id lists. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/global/CStrId.h"
#include "game/CCharClassModifyColliderStateBaseTrack.h"
#include "types.h"

using base::global::CStrId;

// The base track's two words.
struct UnkTrackBaseWords {
    u64 field_0x0;
    u64 field_0x8;
};

// The track's members: six times a string id, the ids split from it, a second id and a word.
struct UnkModifyColliderStateBaseTrackCopy {
    void* vtable;
    UnkTrackBaseWords field_0x8;  // the base track's
    CStrId field_0x18;
    base::UnkStrIdVec field_0x20;
    CStrId field_0x48;
    u32 field_0x50;
    CStrId field_0x58;
    base::UnkStrIdVec field_0x60;
    CStrId field_0x88;
    u32 field_0x90;
    CStrId field_0x98;
    base::UnkStrIdVec field_0xa0;
    CStrId field_0xc8;
    u32 field_0xd0;
    CStrId field_0xd8;
    base::UnkStrIdVec field_0xe0;
    CStrId field_0x108;
    u32 field_0x110;
    CStrId field_0x118;
    base::UnkStrIdVec field_0x120;
    CStrId field_0x148;
    u32 field_0x150;
    CStrId field_0x158;
    base::UnkStrIdVec field_0x160;
    CStrId field_0x188;
    u32 field_0x190;
};
static_assert(sizeof(UnkModifyColliderStateBaseTrackCopy) == 0x198, "UnkModifyColliderStateBaseTrackCopy layout");

extern void* g_unk_7101a1a138[];  // the base track's vtable

void fn_7100d1ffcc(UnkModifyColliderStateBaseTrackCopy* self, const UnkModifyColliderStateBaseTrackCopy* other) {
    self->vtable = g_unk_7101a1a138 + 2;
    self->field_0x8 = other->field_0x8;
    self->vtable = g_unk_7101aa7cb0 + 2;
    new (&self->field_0x18) CStrId(other->field_0x18);
    new (&self->field_0x20) base::UnkStrIdVec(other->field_0x20);
    new (&self->field_0x48) CStrId(other->field_0x48);
    self->field_0x50 = other->field_0x50;
    new (&self->field_0x58) CStrId(other->field_0x58);
    new (&self->field_0x60) base::UnkStrIdVec(other->field_0x60);
    new (&self->field_0x88) CStrId(other->field_0x88);
    self->field_0x90 = other->field_0x90;
    new (&self->field_0x98) CStrId(other->field_0x98);
    new (&self->field_0xa0) base::UnkStrIdVec(other->field_0xa0);
    new (&self->field_0xc8) CStrId(other->field_0xc8);
    self->field_0xd0 = other->field_0xd0;
    new (&self->field_0xd8) CStrId(other->field_0xd8);
    new (&self->field_0xe0) base::UnkStrIdVec(other->field_0xe0);
    new (&self->field_0x108) CStrId(other->field_0x108);
    self->field_0x110 = other->field_0x110;
    new (&self->field_0x118) CStrId(other->field_0x118);
    new (&self->field_0x120) base::UnkStrIdVec(other->field_0x120);
    new (&self->field_0x148) CStrId(other->field_0x148);
    self->field_0x150 = other->field_0x150;
    new (&self->field_0x158) CStrId(other->field_0x158);
    new (&self->field_0x160) base::UnkStrIdVec(other->field_0x160);
    new (&self->field_0x188) CStrId(other->field_0x188);
    self->field_0x190 = other->field_0x190;
}
