#include "base/reflection/UnkReflAttributes.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::reflection::UnkTunable;

bool fn_7100df5740(UnkGameManager*);
extern UnkTunable g_unk_71726cb3f8;

struct UnkPos_b0236f {
    f32 x, y, z;
};

static inline UnkPos_b0236f Pos(const UnkActor* a) {
    return {a->field_0x44, a->field_0x54, a->field_0x64};
}

bool fn_710046971c(CAttack* self, void* arg, u32 mode) {
    if (!fn_7100df5740(g_unk_7101d47310))
        return false;
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    if (target) {
        UnkPos_b0236f t = Pos(target);
        UnkPos_b0236f p = Pos(self->field_0x8);
        f32 dx = t.x - p.x;
        f32 dy = t.y - p.y;
        f32 dz = t.z - p.z;
        f32 dist2 = dx * dx + dy * dy + dz * dz;
        f32 radius = *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_71726cb3f8) + 0x2c);
        radius *= radius;
        if (dist2 > radius)
            return false;
    }
    return fn_7100360b84(self, arg, mode);
}
