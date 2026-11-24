// CCentralUnitCannonBeamMovementComponent's unit (tools/tu_map.py): the beam's hit. A null `other` or the one
// held at +0x230 (+0x10) is no hit; otherwise the beam keeps the hit point, sets its damage request up
// (fn_71003ffe6c), applies the hit (fn_7100ac82c0) and runs the base hit (fn_7100ac4ec0). Names and layouts
// are placeholders.
#include "game/CProjectileMovement.h"
#include "game/CWeaponMovement.h"
#include "types.h"

struct UnkCannonBeamMovement_l3ffe6c;
struct UnkCannonDamageRequest_l3ffe6c;

void CCentralUnitCannonBeamMovementComponent_SetBeamDamage(UnkCannonBeamMovement_l3ffe6c* self, UnkCannonDamageRequest_l3ffe6c* request);

// Where and how the hit landed: nine floats.
struct UnkCannonHitPoint_l3ffd84 {
    f32 values[9];
};

struct UnkCannonHit_l3ffd84 {
    u8 pad_0x0[0x14];
    UnkCannonHitPoint_l3ffd84 point;  // 0x14
};

struct UnkCannonHeld_l3ffd84 {
    u8 pad_0x0[0x10];
    void* other;  // 0x10
};

struct UnkCannonBeamMovement_l3ffd84 {
    u8 pad_0x0[0x230];
    UnkCannonHeld_l3ffd84* held;  // 0x230
    u8 pad_0x238[0x110];
    u8 request[0x68];  // 0x348
    s32 field_0x3b0;
    UnkCannonHitPoint_l3ffd84 point;  // 0x3b4
    u32 field_0x3d8;
    u32 field_0x3dc;
};
static_assert(__builtin_offsetof(UnkCannonBeamMovement_l3ffd84, request) == 0x348, "layout");
static_assert(__builtin_offsetof(UnkCannonBeamMovement_l3ffd84, field_0x3d8) == 0x3d8, "layout");

bool fn_71003ffd84(UnkCannonBeamMovement_l3ffd84* self, void* other, void* a2, void* a3, UnkCannonHit_l3ffd84* hit,
                   void* a5) {
    if (!other)
        return false;
    UnkCannonHeld_l3ffd84* held = self->held;
    if ((held ? held->other : nullptr) == other)
        return false;
    self->point = hit->point;
    self->field_0x3b0 = -1;
    self->field_0x3d8 = 0;
    self->field_0x3dc = 0x238;
    CCentralUnitCannonBeamMovementComponent_SetBeamDamage(reinterpret_cast<UnkCannonBeamMovement_l3ffe6c*>(self),
                  reinterpret_cast<UnkCannonDamageRequest_l3ffe6c*>(self->request));
    fn_7100ac82c0(reinterpret_cast<UnkSplash_b59a4*>(self), other);
    return fn_7100ac4ec0(reinterpret_cast<UnkObj*>(self), other, a2, a3, hit, a5);
}
