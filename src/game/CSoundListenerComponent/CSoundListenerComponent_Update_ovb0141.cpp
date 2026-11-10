// CSoundListenerComponent's unit (tools/tu_map.py): refreshes the cached listener position (+0x58) and angle
// (+0x64) from the owner's (+0x30) transform and, when either moved by more than 0.001 on any axis,
// updates the sound manager (g_unk_7101d4d820) with the position lifted by 100 (fn_710010807c). Names and
// layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;
void fn_710010807c(UnkSoundMgr* mgr, const base::math::CVector3D* pos, const base::math::CVector3D* a,
                   const base::math::CVector3D* b);

struct UnkListenerOwner_b0141 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkListener_b0141 {
    u8 pad_0x0[0x30];
    UnkListenerOwner_b0141* owner;  // 0x30
    u8 pad_0x38[0x14];
    u8 field_0x4c[0xc];
    UnkVec3f pos;    // 0x58
    UnkVec3f angle;  // 0x64
};
static_assert(__builtin_offsetof(UnkListener_b0141, pos) == 0x58, "layout");

static inline bool Near(f32 a, f32 b) {
    return b >= a - 0.001f && b <= a + 0.001f;
}

void fn_710090eac4(UnkListener_b0141* self) {
    UnkListenerOwner_b0141* owner = self->owner;
    UnkVec3f p = owner->xform.GetTranslation();
    bool moved = false;
    if (!(Near(p.x, self->pos.x) && Near(p.y, self->pos.y) && Near(p.z, self->pos.z))) {
        moved = true;
        self->pos = p;
    }
    UnkVec3f angle = owner->xform.GetAngle();
    if (!(Near(angle.x, self->angle.x) && Near(angle.y, self->angle.y) && Near(angle.z, self->angle.z))) {
        moved = true;
        self->angle = angle;
    }
    if (moved) {
        UnkVec3f pos(self->pos.x + 0.0f, self->pos.y + 0.0f, self->pos.z + 100.0f);
        fn_710010807c(g_unk_7101d4d820, reinterpret_cast<const base::math::CVector3D*>(&pos),
                      reinterpret_cast<const base::math::CVector3D*>(&self->pos),
                      reinterpret_cast<const base::math::CVector3D*>(self->field_0x4c));
    }
}
