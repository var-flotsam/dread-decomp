// behaviortree::CShakernautIsTargetDetectedPartiallyCondition's unit (tools/tu_map.py unit 1044): the condition. With the
// Shakernaut AI's flag set it takes the detection limits (fn_71010fc864), projects one of them to the screen and asks
// the render manager (fn_7100219588) when the projection leaves the owner's box. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkDetectLimit {
    f32 x, y, z;
    UnkDetectLimit() : x(0.0f), y(0.0f), z(0.0f) {}
};

struct UnkDetectKind {
    u32 value;
    u32 unk_0x4;
};

struct UnkShakernautAI {
    u8 pad_0x0[0x14810];
    f32 field_0x14810;
    u8 pad_0x14814[0x148a0 - 0x14814];
    bool field_0x148a0;
};

struct UnkDetectOwner {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x240 - 0x1f8];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x37c - 0x248];
    f32 radius;  // 0x37c
};

struct UnkDetectCondition {
    u8 pad_0x0[0x18];
    UnkDetectOwner* owner;
};

struct UnkDetectBounds {
    u8 pad_0x0[0x8c];
    u32 x;  // 0x8c
    u32 y;  // 0x90
};

struct UnkDetectMovement {
    u8 pad_0x0[0x8e8];
    UnkDetectBounds bounds;  // 0x8e8
};

bool fn_7100219588(UnkRenderManager*, const void*, u32, u32);
struct UnkBigFistAI3_ovb0050;
struct UnkBigFistKind_ovb0050;
struct UnkBigFistLimit_ovb0050;

static inline f32 Bits(u32 v) {
    f32 f;
    __builtin_memcpy(&f, &v, 4);
    return f;
}

static inline bool Outside(f32 radius, u32 ua, u32 ub, const CVector2D& screen) {
    f32 a = Bits(ua);
    f32 sx = screen.x;
    if (!(radius + a >= sx) || !(a - radius <= sx))
        return true;
    f32 b = Bits(ub);
    f32 sy = screen.y;
    return !(radius + b >= sy) || !(b - radius <= sy);
}

bool fn_71006abe10(UnkDetectCondition* self) {
    if (!self->owner)
        return false;
    UnkReflObject* raw = self->owner->ai;
    UnkShakernautAI* ai = fn_710009d50c(raw->vf_0x0(), fn_71006ad0a4()) ? reinterpret_cast<UnkShakernautAI*>(raw) : nullptr;
    if (!ai->field_0x148a0)
        return false;
    f32* tunable = &ai->field_0x14810;
    UnkReflObject* movement_raw = self->owner->movement;
    UnkDetectMovement* movement = nullptr;
    if (movement_raw)
        movement = fn_710009d50c(movement_raw->vf_0x0(), fn_71010c74e0()) ? reinterpret_cast<UnkDetectMovement*>(movement_raw) : nullptr;
    u32 kind_of = fn_71010cc3a4(movement);
    f32 gate = *tunable;
    UnkDetectLimit min;
    UnkDetectLimit max;
    bool found;
    {
        UnkDetectKind kind;
        kind.value = 2;
        found = fn_71010fc864(reinterpret_cast<UnkBigFistAI3_ovb0050*>(ai), reinterpret_cast<UnkBigFistKind_ovb0050*>(&kind),
                             reinterpret_cast<UnkBigFistLimit_ovb0050*>(&min), reinterpret_cast<UnkBigFistLimit_ovb0050*>(&max));
    }
    if (!found)
        return false;
    UnkDetectBounds* source = kind_of == 1 ? &movement->bounds : nullptr;
    u32 ua = source->x;
    u32 ub = source->y;
    CVector2D screen;
    if (gate > 0.0f) {
        fn_710007b430(&screen, reinterpret_cast<CVector3D*>(&min));
        f32 radius = self->owner->radius;
        if (Outside(radius, ua, ub, screen))
            return !fn_7100219588(g_unk_7101d47300, &min, 0, 0);
        return false;
    }
    fn_710007b430(&screen, reinterpret_cast<CVector3D*>(&max));
    f32 radius = self->owner->radius;
    if (Outside(radius, ua, ub, screen))
        return !fn_7100219588(g_unk_7101d47300, &max, 0, 0);
    return false;
}
