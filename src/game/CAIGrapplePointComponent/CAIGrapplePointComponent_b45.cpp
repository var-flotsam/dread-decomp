// CAIGrapplePointComponent's unit (tools/tu_map.py): whether the owner's (+0x30) collision container
// (+0x2a0) has an enabled object (flag +0x6a) named by the definition's (+0x90) id at +0x78 whose
// flags (virtual slot 0x58) have bit 7 set. Names are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

struct UnkGrappleOwner_b45a2 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkGrappleDef_b45a2 {
    u8 pad_0x0[0x78];
    base::global::CStrId name;
};

struct UnkGrappleCollider_b45a2 {
    void** vtable;
    u8 pad_0x8[0x62];
    bool enabled;  // 0x6a
    u32 vf_0x58() { return reinterpret_cast<u32 (*)(UnkGrappleCollider_b45a2*)>(vtable[0x58 / 8])(this); }
};

struct UnkAIGrapplePoint_b45a2 {
    u8 pad_0x0[0x30];
    UnkGrappleOwner_b45a2* owner;
    u8 pad_0x38[0x58];
    UnkGrappleDef_b45a2* def;  // 0x90
};

bool fn_7100827a10(UnkAIGrapplePoint_b45a2* self) {
    UnkCollisionContainer* collision = self->owner->collision;
    if (!collision)
        return false;
    auto* c = reinterpret_cast<UnkGrappleCollider_b45a2*>(fn_71008640a4(collision, &self->def->name));
    if (c && c->enabled && (c->vf_0x58() & 0x80))
        return true;
    return false;
}
