// CProjectileMovement unit (tools/tu_map.py unit 1623): keeps a counted weak handle (+0x230) to
// `target` (none for null) and records the target's position, its transform's translation
// (include/game/UnkXformNode.h), or zero when the handle points at nothing. Names and layouts are
// placeholders.
#include "game/UnkTU2523.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"

// The target as the position read sees it: its transform at +0x38.
struct UnkProjectileTargetXform_b46a3 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkProjectileMovement3_b46a3 {
    u8 pad_0x0[0x230];
    UnkWeakHandle* target;  // 0x230
    u8 pad_0x238[0x28c];
    UnkVec3f target_position;  // 0x4c4
};

void fn_7100ac397c(UnkProjectileMovement3_b46a3* self, UnkWeakTarget* target) {
    UnkWeakHandle* h = target ? fn_7100d78378(target) : nullptr;
    AssignRef(&self->target, h);
    UnkWeakTarget* t = h ? h->target : nullptr;
    self->target_position = t ? reinterpret_cast<UnkProjectileTargetXform_b46a3*>(t)->xform.GetTranslation()
                              : UnkVec3f(0.0f, 0.0f, 0.0f);
}
