// CCharClassFanComponent's unit (tools/tu_map.py, unit 1336): the fan's push on the player. A player whose
// character class has the ignore-gale track gets none; otherwise the strength falls off along the fan's
// axis between its radii and points the way the axis does. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::math::CVector3D;

// The player as this function reads it.
struct UnkFanPlayer_l87ee04 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1a8];
    void* field_0x210;
    u8 pad_0x218[0x8];
    UnkAnimationComponent* char_class;  // 0x220
};
static_assert(__builtin_offsetof(UnkFanPlayer_l87ee04, char_class) == 0x220, "");

struct UnkFanComponent_l87ee04 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0xa88];
    f32 outer_radius;  // 0xac0
    f32 inner_radius;  // 0xac4
    f32 max_distance;  // 0xac8
    u8 pad_0xacc[4];
    CVector3D axis;  // 0xad0
};
static_assert(__builtin_offsetof(UnkFanComponent_l87ee04, axis) == 0xad0, "");

void fn_7100a47124(UnkFanComponent_l87ee04* self);
void fn_7100f0e478(void* player_info, f32 speed);

static inline f32 Min1(f32 v) {
    return !(v >= 1.0f) ? v : 1.0f;
}

void fn_710087ee04(UnkFanComponent_l87ee04* self) {
    fn_7100a47124(self);
    UnkFanPlayer_l87ee04* player = reinterpret_cast<UnkFanPlayer_l87ee04*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    if (player->char_class && fn_710117b348(player->char_class, fn_7100c2d5d8())) {
        fn_7100f0e478(g_unk_7101d47310->field_0x38, 0.0f);
        return;
    }

    UnkActor* owner = self->owner;
    f32 max_sq = self->max_distance * self->max_distance;
    CVector3D diff = CVector3D{player->x, player->y, player->z} -
                     CVector3D{owner->field_0x44, owner->field_0x54, owner->field_0x64};
    const CVector3D& axis = self->axis;
    // How far along the axis the player is, squared.
    f32 along_sq = LengthSq(axis * (Dot(diff, axis) / LengthSq(axis)));
    f32 ax = axis.x;

    f32 inner_sq = self->inner_radius * self->inner_radius;
    f32 near = along_sq / max_sq;
    f32 far = (along_sq - inner_sq) / (self->outer_radius * self->outer_radius - inner_sq);

    f32 speed = (1.0f - Min1(near)) * 700.0f + 1400.0f;
    if (near <= 0.0f)
        speed = 2100.0f;
    f32 far_speed = 1400.0f;
    if (player->field_0x210)
        far_speed = 840.0f;
    f32 far_scale = 1.0f - Min1(far);
    if (far <= 0.0f)
        far_scale = 1.0f;
    if (along_sq > max_sq)
        speed = far_speed * far_scale;

    s32 sign = (ax > 0.0f) - (ax < 0.0f);
    fn_7100f0e478(g_unk_7101d47310->field_0x38, speed * sign);
}
