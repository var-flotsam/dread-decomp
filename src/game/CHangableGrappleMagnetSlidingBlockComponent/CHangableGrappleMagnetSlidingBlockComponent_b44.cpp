// CHangableGrappleMagnetSlidingBlockComponent (tools/tu_map.py; the name from reflection): sets the
// block's state (fn_7100825644), then, when its definition (+0xa0) names a collider (+0x70), sets the
// same flag on that collider of the owner's collision container (+0x2a0; fn_71008640a4,
// fn_7100d35a98). Names and layout are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

struct UnkGrappleBlockDef_b44a2 {
    u8 pad_0x0[0x70];
    base::global::CStrId collider;
};

struct UnkGrappleBlockOwner_b44a2 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkGrappleBlock_b44a2 {
    u8 pad_0x0[0x30];
    UnkGrappleBlockOwner_b44a2* owner;
    u8 pad_0x38[0x68];
    UnkGrappleBlockDef_b44a2* def;
};

void fn_7100825644(UnkGrappleBlock_b44a2* self, bool on);

void fn_71008244c0(UnkGrappleBlock_b44a2* self, bool on) {
    fn_7100825644(self, on);
    const base::global::CStrId* name = &self->def->collider;
    if (name->data == &g_unk_7101d47108->empty)
        return;
    if (UnkCollisionObject* collider = fn_71008640a4(self->owner->collision, name))
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), on);
}
