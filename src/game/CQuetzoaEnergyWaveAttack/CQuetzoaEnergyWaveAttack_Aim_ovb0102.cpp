// CQuetzoaEnergyWaveAttack (tools/tu_map.py unit 968): aiming the energy wave at the owner's target
// (fn_71006455bc, inlined by the start and the update), the start (the owner's +0x298 sub-object gets a
// kind-5 parameter block; the spawn point is fn_7100645568's offset) and the update that keeps a new
// direction when fn_71006457c4 allows it. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

struct UnkVec3_ovb0102 {
    f32 x, y, z;
};

struct UnkEnergyWaveAttack_ovb0102 {
    void** vtable;
    UnkActor* owner;  // +0x8
    u8 pad_0x10[0x20];
    UnkVec3_ovb0102 dir;  // +0x30
};

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase*);

static inline f32 Sign(f32 f) {
    return (f32)((s32)(f > 0.0f) - (s32)(f < 0.0f));
}

// The direction from the wave's origin (115 units ahead along the facing sign, 20 up from `pos`)
// to the owner's target, mirrored by the facing; false when there is no target or the angle is more
// than 70 degrees off the horizontal.
static inline bool Aim(UnkEnergyWaveAttack_ovb0102* self, const UnkVec3_ovb0102* pos, const f32* facing,
                       UnkVec3_ovb0102* dir) {
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return false;
    f32 sign = Sign(*facing);
    f32 x = pos->x + sign * 115.0f;
    f32 y = pos->y + 20.0f;
    f32 tx = target->field_0x44;
    f32 ty = target->field_0x54;
    f32 dx = tx - x;
    f32 dy = ty - y;
    f32 angle = atan2f(dy, dx * sign);
    if (!(angle >= -1.2217305f && angle <= 1.2217305f))
        return false;
    f32 c = cosf(angle) * sign;
    f32 s = sinf(angle);
    dir->x = c;
    dir->y = s;
    dir->z = 0.0f;
    return true;
}

static inline void Offset(UnkVec3_ovb0102* pos, const f32* facing) {
    f32 sign = Sign(*facing);
    UnkVec3_ovb0102 offset = {sign * -211.0f, 78.0f, 0.0f};
    pos->x += offset.x;
    pos->y += offset.y;
    pos->z += offset.z;
}

static inline f32 Facing(UnkActor* owner) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38)).m[2];
}

// Starts the wave: the owner's +0x298 sub-object gets a kind-5 parameter block, and the wave aims
// from its spawn point (fn_7100645568's offset from the owner) at the target.
void fn_7100645408(UnkEnergyWaveAttack_ovb0102* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    UnkActor* owner = self->owner;
    if (void* target = owner->field_0x298) {
        UnkParams params;
        __builtin_memset(params.field_0x8, 0, 13);
        params.field_0x30 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        params.field_0x18 = 0;
        params.field_0x1c = 1;
        fn_71006f4b0c(target, &params, 1);
        owner = self->owner;
    }
    UnkVec3_ovb0102 pos = {owner->field_0x44, owner->field_0x54, owner->field_0x64};
    f32 facing = Facing(owner);
    Offset(&pos, &facing);
    Aim(self, &pos, &facing, &self->dir);
}

bool fn_71006455bc(UnkEnergyWaveAttack_ovb0102* self, const UnkVec3_ovb0102* pos, const f32* facing,
                   UnkVec3_ovb0102* dir) {
    return Aim(self, pos, facing, dir);
}

// Whether the wave can travel from `pos` along `dir`.
bool fn_71006457c4(UnkEnergyWaveAttack_ovb0102* self, const UnkVec3_ovb0102* pos, const UnkVec3_ovb0102* dir);

// Updates the wave: re-aims from the owner's position at the target and keeps the new direction
// when the wave can travel along it.
void fn_7100645698(UnkEnergyWaveAttack_ovb0102* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    UnkActor* owner = self->owner;
    f32 x = owner->field_0x44;
    f32 y = owner->field_0x54;
    f32 z = owner->field_0x64;
    UnkVec3_ovb0102 pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    f32 facing = Facing(owner);
    UnkVec3_ovb0102 dir;
    dir.x = 0.0f;
    dir.y = 0.0f;
    dir.z = 0.0f;
    if (Aim(self, &pos, &facing, &dir) && fn_71006457c4(self, &pos, &dir)) {
        self->dir.x = dir.x;
        self->dir.y = dir.y;
        self->dir.z = dir.z;
    }
}
