// CKraidShockerSplashMovementComponent's unit: after the base movement update the owner faces along the
// velocity; then, while both ends are present, the splash's four effect parts follow the two ends as a timer
// runs, and once one end is gone the parts are switched off. Names and layouts are placeholders.
#include <math.h>

#include "base/color/CColor4F.h"
#include "game/CFXComponent.h"
#include "game/UnkMovementComponent.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU133.h"
#include "game/UnkTU134.h"
#include "game/UnkTU341.h"
#include "game/UnkTU53.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::color::CColor4F;

struct UnkSlots;
bool fn_71001c2a2c(UnkSlots* self, u32 kind, const CColor4F* color, u32 mode);

struct UnkSplashOwner_l607270 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform transform;  // 0x38
    u8 pad_0x94[0x1ec];
    UnkWorld* world;  // 0x280
    u8 pad_0x288[0x138];
    bool field_0x3c0;

    void vf_0x118(UnkXform* xform, s32* mode) {
        reinterpret_cast<void (*)(UnkSplashOwner_l607270*, UnkXform*, s32*)>(vtable[0x118 / 8])(this, xform, mode);
    }
};
static_assert(__builtin_offsetof(UnkSplashOwner_l607270, field_0x3c0) == 0x3c0, "owner layout");

struct UnkSplashEnd_l607270 {
    u8 pad_0x0[0x10];
    UnkSplashOwner_l607270* actor;  // 0x10
};

struct UnkSplashMovement_l607270 {
    u8 pad_0x0[0x30];
    UnkSplashOwner_l607270* owner;  // 0x30
    u8 pad_0x38[0x480];
    f32 vx, vy, vz;  // 0x4b8
    u8 pad_0x4c4[0x114];
    UnkSplashEnd_l607270* left;   // 0x5d8
    UnkSplashEnd_l607270* right;  // 0x5e0
    f32 time;                     // 0x5e8
    f32 duration;                 // 0x5ec
};
static_assert(__builtin_offsetof(UnkSplashMovement_l607270, left) == 0x5d8, "movement layout");

struct UnkSplashPart_l607270 {
    u8 pad_0x0[0xb];
    u8 kind;
};

// The world's part with this name hash, or null when there is none or `Shown` says it is not shown.
template <bool (*Shown)(UnkObject*, u8)>
static inline UnkSwitchObject* PartOf(UnkWorld* world, u64 hash) {
    UnkPart* part = fn_7101152b90(world, hash);
    if (part && !Shown(reinterpret_cast<UnkObject*>(world), reinterpret_cast<UnkSplashPart_l607270*>(part)->kind))
        part = nullptr;
    return reinterpret_cast<UnkSwitchObject*>(part);
}

// Turns the owner to face along the velocity (its z angle, offset by pi).
static inline void FaceAlongVelocity(UnkSplashMovement_l607270* self) {
    f32 inv = 1.0f / (sqrtf(self->vx * self->vx + self->vy * self->vy + self->vz * self->vz) + 0.0001f);
    f32 x = self->vx * inv;
    f32 y = self->vy * inv;
    UnkSplashOwner_l607270* owner = self->owner;
    f32 angle = atan2f(y, x) + 3.1415927f;
    owner->transform.SetAngle(UnkVec3f(0.0f, 0.0f, angle));
    s32 mode = 2;
    owner->vf_0x118(&owner->transform, &mode);
    owner->field_0x3c0 = true;
}

static inline bool IsPresent(UnkSplashEnd_l607270* end) { return end && end->actor; }

void fn_7100607270(UnkSplashMovement_l607270* self, f32 dt) {
    fn_71010d1a90(reinterpret_cast<UnkMovementComponent*>(self), dt);
    FaceAlongVelocity(self);

    UnkWorld* world = self->owner->world;
    if (!world)
        return;
    UnkSwitchObject* leftBeam = PartOf<fn_710114e680>(world, 0x89f328dc37ac167bull);
    UnkSwitchObject* rightBeam = PartOf<fn_710114e680>(world, 0x8c9dfb3ece4e323cull);
    UnkSwitchObject* rightFx = PartOf<fn_710114e668>(world, 0xa0cda40259a7a253ull);
    UnkSwitchObject* leftFx = PartOf<fn_710114e668>(world, 0xa5a377e0a0458614ull);

    if (IsPresent(self->left) && IsPresent(self->right) && rightFx && leftFx) {
        self->time += dt;
        f32 t = self->time / self->duration;
        t = !(t <= 0.0f) ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
        CColor4F color(1.0f, 1.0f, 1.0f, t <= 0.0f ? 0.0f : t);
        f32 eased = fn_7100076020(t, "2");

        UnkVec3f pos = self->right->actor->transform.GetTranslation();
        UnkVec3f fxPos(pos.x + 50.0f, pos.y + 0.0f, pos.z + 0.0f);
        fn_7100146d84(reinterpret_cast<UnkFxObject_ovb0041*>(rightFx), &fxPos);
        fn_7100146e60(rightFx, true);
        f32 leftX = pos.x - 50.0f;
        f32 offset = eased * -800.0f;
        pos.x = offset + leftX;
        fn_7100145f28(leftBeam, &pos);
        fn_71001c2a2c(reinterpret_cast<UnkSlots*>(leftBeam->field_0x160), 0, &color, 0);

        pos = self->left->actor->transform.GetTranslation();
        fxPos = UnkVec3f(pos.x - 50.0f, pos.y, pos.z);
        fn_7100146d84(reinterpret_cast<UnkFxObject_ovb0041*>(leftFx), &fxPos);
        fn_7100146e60(leftFx, true);
        pos.x = pos.x + 50.0f - offset;
        fn_7100145f28(rightBeam, &pos);
        fn_71001c2a2c(reinterpret_cast<UnkSlots*>(rightBeam->field_0x160), 0, &color, 0);
    }

    if (!IsPresent(self->left) && self->time > 0.0f) {
        fn_7100145e94(leftBeam, false);
        fn_7100145e94(rightBeam, false);
        fn_7100146e60(rightFx, false);
        fn_7100146e60(leftFx, false);
        fn_7100232c68(self->owner);
    }
}
