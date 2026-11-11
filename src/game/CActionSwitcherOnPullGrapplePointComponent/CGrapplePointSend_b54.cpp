// Two pull-grapple-point handlers that run the pullable grapple point's fn_710082cce8, then send
// the owner's (+0x30) sub-object at +0x220 a hash with mode 0 (as UnkSpitSend,
// src/game_s2/UnkSpitSend.h): fn_710082bec4 (CPlatformTrapGrapplePointComponent's unit) a fixed
// hash; fn_71008262c4 (CActionSwitcherOnPullGrapplePointComponent's unit) the hash of the string
// id at +0x80 of the object at +0x90, unless that id is the pool's empty one. fn_7100826090 (the
// same unit) is the pull-state handler: past fn_710082cacc, state 1 sets the flag at +0x98 and,
// when there is a sub-object to send to, sends the hash of the string id at +0xa0 (with the other
// second descriptor, UnkNamedSend_b54.h), calls fn_710082cee4(0) and fn_7100d7c828 on the owner,
// then fn_710082cf08(1). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "../UnkNamedSend_b54.h"
#include "base/global/CStrId.h"
#include "game/CPullableGrapplePointComponent.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkPullableGrapplePoint_b54;
void fn_710082cce8(UnkPullableGrapplePoint_b54* self);
void fn_710082cacc(UnkPullableGrapplePoint_b54* self, s32 state);

extern const u64 g_unk_710164ac20 __attribute__((visibility("hidden")));

struct UnkGrappleOwner_b54 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkGrappleIdHolder_b54 {
    u8 pad_0x0[0x80];
    base::global::UnkStringData* id;  // CStrId
};

struct UnkGrapplePoint_b54 {
    u8 pad_0x0[0x30];
    UnkGrappleOwner_b54* owner;
    u8 pad_0x38[0x58];
    UnkGrappleIdHolder_b54* field_0x90;
    bool field_0x98;
    base::global::UnkStringData* field_0xa0;  // CStrId
};

static inline u64 StrIdHash_b54(base::global::UnkStringData* data) {
    return *reinterpret_cast<u64*>(reinterpret_cast<char*>(data) + 0x30);
}

void fn_710082bec4(UnkGrapplePoint_b54* self) {
    fn_710082cce8(reinterpret_cast<UnkPullableGrapplePoint_b54*>(self));
    UnkSpitSend(self->owner->field_0x220, 0xc710a77d9222340d, 0xf4061ff25a2163d6);
}

void fn_71008262c4(UnkGrapplePoint_b54* self) {
    fn_710082cce8(reinterpret_cast<UnkPullableGrapplePoint_b54*>(self));
    base::global::UnkStringData* id = self->field_0x90->id;
    if (id != &g_unk_7101d47108->empty)
        UnkSpitSend(self->owner->field_0x220, StrIdHash_b54(id), 0xf4061ff25a2163d6);
}

void fn_7100826090(UnkGrapplePoint_b54* self, s32 state) {
    UnkPullableGrapplePoint_b54* base = reinterpret_cast<UnkPullableGrapplePoint_b54*>(self);
    fn_710082cacc(base, state);
    if (state != 1)
        return;
    self->field_0x98 = true;
    if (void* target = self->owner->field_0x220) {
        UnkSpitSend_974c(target, StrIdHash_b54(self->field_0xa0), 0xf4061ff25a2163d6);
        fn_710082cee4(base, false);
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164ac20, false);
    }
    fn_710082cf08(base, true);
}
