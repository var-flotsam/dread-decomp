// From unit 2452 (CCharClassModifyColliderStateBaseTrack): the track's destructor, which releases
// its six groups of string ids and id lists, last to first, then runs the base track's.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CTrack.h"
#include "game/CCharClassModifyColliderStateBaseTrack.h"
#include "types.h"

using base::global::CStrId;

// A list of string ids that frees its array when destroyed.
struct UnkColliderStateIdList : base::global::CRntVector<CStrId> {
    ~UnkColliderStateIdList() { Free(); }
};

// The track's own members: six times a string id, the ids split from it, a second id and a word.
struct UnkModifyColliderStateBaseTrack {
    void* vtable;
    u8 pad_0x8[0x10];  // the base track's
    CStrId field_0x18;
    UnkColliderStateIdList field_0x20;
    CStrId field_0x48;
    u32 field_0x50;
    CStrId field_0x58;
    UnkColliderStateIdList field_0x60;
    CStrId field_0x88;
    u32 field_0x90;
    CStrId field_0x98;
    UnkColliderStateIdList field_0xa0;
    CStrId field_0xc8;
    u32 field_0xd0;
    CStrId field_0xd8;
    UnkColliderStateIdList field_0xe0;
    CStrId field_0x108;
    u32 field_0x110;
    CStrId field_0x118;
    UnkColliderStateIdList field_0x120;
    CStrId field_0x148;
    u32 field_0x150;
    CStrId field_0x158;
    UnkColliderStateIdList field_0x160;
    CStrId field_0x188;
    u32 field_0x190;
};
static_assert(sizeof(UnkModifyColliderStateBaseTrack) == 0x198, "UnkModifyColliderStateBaseTrack layout");

void fn_7100d1f05c(UnkModifyColliderStateBaseTrack* self) {
    self->vtable = g_unk_7101aa7cb0 + 2;
    self->field_0x188.~CStrId();
    self->field_0x160.~UnkColliderStateIdList();
    self->field_0x158.~CStrId();
    self->field_0x148.~CStrId();
    self->field_0x120.~UnkColliderStateIdList();
    self->field_0x118.~CStrId();
    self->field_0x108.~CStrId();
    self->field_0xe0.~UnkColliderStateIdList();
    self->field_0xd8.~CStrId();
    self->field_0xc8.~CStrId();
    self->field_0xa0.~UnkColliderStateIdList();
    self->field_0x98.~CStrId();
    self->field_0x88.~CStrId();
    self->field_0x60.~UnkColliderStateIdList();
    self->field_0x58.~CStrId();
    self->field_0x48.~CStrId();
    self->field_0x20.~UnkColliderStateIdList();
    self->field_0x18.~CStrId();
    fn_710006b5dc(self);
}