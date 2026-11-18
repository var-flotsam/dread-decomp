// CYamplotXAIComponent unit: the melee choice. After the base choice, a pending melee (+0x14760) is
// consumed; unless the target stands between the two path points while facing back along the path,
// the request gets "meleesuccess2m". Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "game/UnkActor.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkMeleeRequest_l1065bc {
    u64 key;  // 0x0
    u8 pad_0x8[0x1c];
    u32 field_0x24;
    u8 pad_0x28[0x8];
    CStrId id;  // 0x30
};

struct UnkMeleeSelf_l1065bc;

// The target's hit state (the actor's +0x208).
struct UnkYamplotXHitState_l6f0f00 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

struct UnkYamplotXAI_l6f0f00 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x14760 - 0x38];
    bool melee_pending;  // 0x14760
    u8 pad_0x14761[0x147cc - 0x14761];
    CVector2D path_start;  // 0x147cc
    u8 pad_0x147d4[0x147e8 - 0x147d4];
    CVector2D path_end;  // 0x147e8
};
static_assert(__builtin_offsetof(UnkYamplotXAI_l6f0f00, path_end) == 0x147e8, "UnkYamplotXAI_l6f0f00 layout");

void fn_71006f0f00(UnkYamplotXAI_l6f0f00* self, s32 mode, UnkMeleeRequest_l1065bc* request, bool* out) {
    fn_71011065bc(reinterpret_cast<UnkMeleeSelf_l1065bc*>(self), mode, request, out);
    if (!self->melee_pending)
        return;
    self->melee_pending = false;

    if (UnkActor* target = fn_7100d7c990(self->owner)) {
        if (target->field_0x208 != nullptr) {
            if (static_cast<UnkYamplotXHitState_l6f0f00*>(target->field_0x208)->field_0xf9)
                return;
            if (fn_71009e12d0(target->field_0x208))
                return;
        }
        f32 target_x = target->field_0x44;
        f32 owner_x = self->owner->field_0x44;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(target->field_0x38));
        CVector3D facing = {m.m[2], m.m[6], m.m[10]};
        CVector2D start = self->path_start;
        CVector2D end = self->path_end;
        CVector3D path = {end.x - start.x, end.y - start.y, 0.0f};
        if (base::math::Dot(facing, base::math::Normalized(path)) < 0.0f) {
            if (target_x > owner_x && target_x < start.x && end.x < target_x)
                return;
            if (target_x < owner_x && target_x > start.x && end.x > target_x)
                return;
        }
    }

    static CStrId s_success("meleesuccess2m", true);
    request->id = s_success;
    request->key = s_success.data->Key();
    request->field_0x24 = 1;
    *out = true;
}
