// CBaseBigFistAIComponent's unit (tools/tu_map.py, unit 591): picks the big fist's close attack: when
// the target is within reach (fn_71010fc778's value under 250) and within 170 units, the request gets
// the attack id; with no id picked it falls back to the base melee choice. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "game/UnkActor.h"
#include "game/UnkMeleeRequest_l1065bc.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

extern CStrId g_unk_71726c0f68 __attribute__((visibility("hidden")));

struct UnkMeleeSelf_l1065bc;

struct UnkBigFistAI_l3e6ccc {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

static inline CVector3D Position(const UnkActor* actor) {
    return CVector3D{actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

void fn_71003e6ccc(UnkBigFistAI_l3e6ccc* self, s32 mode, UnkMeleeRequest_l1065bc* request, bool* out) {
    UnkActor* target = fn_7100d7c990(self->owner);
    f32 reach = 0.0f;
    if (target && fn_71010fc778(self, &reach)) {
        UnkActor* owner = self->owner;
        CVector3D target_pos = Position(target);
        CVector3D owner_pos = Position(owner);
        // The original fetches the owner's matrix here and does not use it.
        fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
        if (reach < 250.0f && base::math::LengthSq(target_pos - owner_pos) < 28900.0f) {
            request->id = g_unk_71726c0f68;
            request->key = g_unk_71726c0f68.data->Key();
            request->field_0x24 = 1;
            *out = true;
        }
    }
    if (request->id.data == &g_unk_7101d47108->empty)
        fn_71011065bc(reinterpret_cast<UnkMeleeSelf_l1065bc*>(self), mode, request, out);
}
