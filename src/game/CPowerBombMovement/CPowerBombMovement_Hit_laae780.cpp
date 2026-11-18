// CPowerBombMovement unit (tools/tu_map.py, unit 1612): a hit on the player while in morph ball. Each of the
// twelve points around the bomb that is within its radius of the hit and not hit yet is marked, and the
// morph ball is pushed with that point's scale. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CWeaponMovement.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkMorphBallMovement_laae780;
void fn_710094bfe8(UnkMorphBallMovement_laae780* movement, const CVector3D* pos, const CVector2D* scale);

// The twelve points around the bomb (offsets before its rotation) and the scales the morph ball is
// pushed with from each, on the two kinds of ground.
__attribute__((visibility("hidden"))) extern const UnkVec3f g_unk_710164b6f8[12];
__attribute__((visibility("hidden"))) extern const CVector2D g_unk_710164b788[12];
__attribute__((visibility("hidden"))) extern const CVector2D g_unk_710164b7e8[12];

struct UnkPowerBombTuning_laae780 {
    u8 pad_0x0[0x35];
    bool field_0x35;
};
extern UnkPowerBombTuning_laae780 g_unk_7172753dc0;

struct UnkPowerBombGame_laae780 {
    u8 pad_0x0[0xc4];
    bool field_0xc4;
    u8 pad_0xc5;
    bool field_0xc6;
    u8 pad_0xc7;
    s32 field_0xc8;
    u8 pad_0xcc[0x40];
    bool field_0x10c;
};

struct UnkPowerBombOwner_laae780 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkPowerBombActor_laae780 {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
};

struct UnkPowerBombHit_laae780 {
    u8 pad_0x0[0x14];
    CVector3D pos;  // 0x14
};

// The twelve points' radii and whether each has hit yet.
struct UnkPowerBombRing_laae780 {
    f32 radius[12];
    bool hit[12];
};

// The rotation of `v` by `angle` (the helper takes the angles where its declaration says a matrix).
inline UnkVec3f Rotate(const UnkVec3f& v, const UnkVec3f& angle) {
    return fn_710007a400(v, reinterpret_cast<const UnkMatrix44&>(angle));
}

struct UnkPowerBombMovement_laae780 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkPowerBombOwner_laae780* owner;  // 0x30
    u8 pad_0x38[0x4f4 - 0x38];
    UnkPowerBombRing_laae780 ring;  // 0x4f4

    void vf_0x2a8(UnkPowerBombActor_laae780* actor, UnkPowerBombHit_laae780* info) {
        reinterpret_cast<void (*)(UnkPowerBombMovement_laae780*, UnkPowerBombActor_laae780*,
                                  UnkPowerBombHit_laae780*)>(vtable[0x2a8 / 8])(this, actor, info);
    }
    void vf_0x2c0(UnkPowerBombActor_laae780* actor, void* arg2, void* arg3, UnkPowerBombHit_laae780* info) {
        reinterpret_cast<void (*)(UnkPowerBombMovement_laae780*, UnkPowerBombActor_laae780*, void*, void*,
                                  UnkPowerBombHit_laae780*)>(vtable[0x2c0 / 8])(this, actor, arg2, arg3, info);
    }
};

void fn_7100aae780(UnkPowerBombMovement_laae780* self, UnkPowerBombActor_laae780* actor, void* arg2, void* arg3,
                   UnkPowerBombHit_laae780* info) {
    // Only a hit on the player is the bomb's own.
    if (fn_7100dec788(g_unk_7101d47310) != reinterpret_cast<UnkPlayer*>(actor)) {
        self->vf_0x2c0(actor, arg2, arg3, info);
        return;
    }
    UnkReflObject* movement = actor->movement;
    if (!movement || !fn_710009d50c(movement->vf_0x0(), fn_710093b204()))
        return;

    auto* game = reinterpret_cast<UnkPowerBombGame_laae780*>(g_unk_7101d47310);
    bool alt = (game->field_0xc8 != 0 || game->field_0x10c) && game->field_0xc4 && !game->field_0xc6;
    bool handled = fn_7100ac79d8(self, actor, info);
    if (!handled)
        self->vf_0x2a8(actor, info);

    CVector2D scale;
    scale.x = 1.0f;
    scale.y = 1.0f;
    bool push = !handled;
    if (g_unk_7172753dc0.field_0x35) {
        scale.x = 0.7f;
        scale.y = 0.8f;
        for (s32 i = 0; i < 12; ++i) {
            UnkXform& xform = self->owner->xform;
            UnkVec3f origin = xform.GetTranslation();
            CVector2D at;
            CVector3D point;
            UnkVec3f offset = Rotate(g_unk_710164b6f8[i], xform.GetAngle());
            point.x = origin.x + offset.x;
            point.y = origin.y + offset.y;
            point.z = offset.z + 0.0f;
            fn_710007b430(&at, &point);
            CVector2D hit_at;
            fn_710007b430(&hit_at, &info->pos);
            f32 dx = at.x - hit_at.x;
            f32 dy = at.y - hit_at.y;
            UnkPowerBombRing_laae780& ring = self->ring;
            if (dx * dx + dy * dy <= ring.radius[i] * ring.radius[i] && !ring.hit[i]) {
                ring.hit[i] = true;
                push = true;
                if (alt) {
                    scale.x = g_unk_710164b7e8[i].x;
                    scale.y = g_unk_710164b7e8[i].y;
                } else {
                    scale.x = g_unk_710164b788[i].x;
                    scale.y = g_unk_710164b788[i].y;
                }
            }
        }
    }
    if (push) {
        UnkXform& xform = self->owner->xform;
        CVector3D pos;
        pos.x = xform.m[0][3];
        pos.y = xform.m[1][3];
        pos.z = 0.0f;
        fn_710094bfe8(reinterpret_cast<UnkMorphBallMovement_laae780*>(movement), &pos, &scale);
    }
}
