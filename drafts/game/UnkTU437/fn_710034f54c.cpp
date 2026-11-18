// UnkTU437 (tools/tu_map.py unit 437, the CAimComponent unit): the direction a shot leaves in. The fixed one at
// +0x984 while "2mb" plays (unless the player is in a cell that allows it), the first column of the "shot"
// node's matrix while a CCharClassUseDC_ShotOrientationTrack plays, else the aim direction for `aim` (1 up
// to 5 down), mirrored when the owner faces left. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

extern "C" f32 sqrtf(f32);

bool fn_71009e12d0(void*);

struct UnkShotPlayerState_l34f54c {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkShotPlayer_l34f54c {
    void** vtable;
    u8 pad_0x8[0x2d08 - 0x8];
    UnkShotPlayerState_l34f54c* state;  // 0x2d08

    UnkNavMeshCell* Cell() { return reinterpret_cast<UnkNavMeshCell* (*)(UnkShotPlayer_l34f54c*)>(vtable[0x2d0 / 8])(this); }
    bool vf_0x3b8() { return reinterpret_cast<bool (*)(UnkShotPlayer_l34f54c*)>(vtable[0x3b8 / 8])(this); }
};

struct UnkShotOwner_l34f54c {
    u8 pad_0x0[0x38];
    UnkXform transform;  // 0x38
    u8 pad_0x94[0x208 - 0x94];
    UnkShotPlayer_l34f54c* player;  // 0x208
    u8 pad_0x210[0x220 - 0x210];
    UnkAnimationComponent* animation;  // 0x220
    UnkEntity* model;  // 0x228
};

struct UnkShotAim_l34f54c {
    u8 pad_0x0[0x30];
    UnkShotOwner_l34f54c* owner;  // 0x30
    u8 pad_0x38[0x984 - 0x38];
    UnkVec3f fixed_direction;  // 0x984
    bool has_fixed_direction;  // 0x990
};

// The record a CCharClassUseDC_ShotOrientationTrack query resolves to.
struct UnkShotOrientation_l34f54c {
    u8 pad_0x0[0x18];
    bool flat;  // 0x18: keep the direction in the xy plane
};

// Whether the player stands in a navmesh cell that lets it shoot freely while "2mb" plays.
static inline bool InFreeShotCell(UnkShotAim_l34f54c* self) {
    UnkShotPlayer_l34f54c* player = self->owner->player;
    if (!player || !player->state || player->state->kind != 3)
        return false;
    UnkNavMeshCell* cell = player->Cell();
    return cell && fn_71002b005c(cell);
}

// The direction for an aim of 1 (up) to 5 (down); any other aim leaves `out` as it is.
static inline void SetAimDirection(UnkVec3f* out, s32 aim) {
    f32 x, y;
    switch (aim) {
    case 1:
        x = 0.0f;
        y = 1.0f;
        break;
    case 2:
        x = 0.70710677f;
        y = 0.70710677f;
        break;
    case 3:
        x = 1.0f;
        y = 0.0f;
        break;
    case 4:
        x = 0.70710677f;
        y = -0.70710677f;
        break;
    case 5:
        x = 0.0f;
        y = -1.0f;
        break;
    default:
        return;
    }
    *out = UnkVec3f(x, y, 0.0f);
}

void fn_710034f54c(UnkShotAim_l34f54c* self, s32 aim, UnkVec3f* out) {
    UnkAnimationComponent* animation = self->owner->animation;
    UnkShotPlayer_l34f54c* player = self->owner->player;
    UnkTrackQuery query = {fn_7100d34f18(), nullptr, -1.0f};
    if (animation)
        fn_710117b08c(animation, &query);

    static const CStrId s_2mb("2mb", true);
    if (self->has_fixed_direction &&
        fn_710117af10(reinterpret_cast<UnkStruct_710117af10*>(animation),
                      reinterpret_cast<UnkCls_710117af10**>(const_cast<CStrId*>(&s_2mb)), false) &&
        !InFreeShotCell(self)) {
        if (self->has_fixed_direction)
            *out = self->fixed_direction;
        return;
    }

    if (query.result) {
        UnkMatrix44 m;
        __builtin_memset(&m, 0, sizeof(m));
        m.m[15] = 1.0f;
        m.m[10] = 1.0f;
        m.m[5] = 1.0f;
        m.m[0] = 1.0f;
        UnkEntity* model = self->owner->model;
        // The table's id at +0xd58; the shared layout does not name it yet.
        UnkGameManagerIdTable& table = GameManagerIdTable();
        u64 key = reinterpret_cast<const CStrId*>(&table.pad_0xd48[0xd58 - 0xd48])->data->Key();
        if (fn_71011735e8(model, &key, &m, 0)) {
            *out = UnkVec3f(m.m[0], m.m[4], m.m[8]);
            if (out->z < 1.0f && reinterpret_cast<UnkShotOrientation_l34f54c*>(query.result)->flat) {
                UnkVec3f flat(out->x, out->y, 0.0f);
                f32 scale = 1.0f / (sqrtf(flat.x * flat.x + flat.y * flat.y + flat.z * flat.z) + 0.0001f);
                *out = UnkVec3f(scale * flat.x, scale * flat.y, scale * flat.z);
            }
        }
        return;
    }

    bool straight = player && fn_71009e12d0(player) && !player->vf_0x3b8();
    *out = UnkVec3f(1.0f, 0.0f, 0.0f);
    if (!straight)
        SetAimDirection(out, aim);
    if (self->owner->transform.GetAngle().y < 0.0f)
        out->x = -out->x;
}
