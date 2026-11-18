// CScorpiusAIComponent unit: the component's update. After the base and own steps it runs one of two
// steps chosen by a tunable mode, then, while the mask is up (+0x14cc0), either times the regenerating
// mask track or starts the mask step (fn_7100695f64) when the owner faces its target; then it updates
// a collider, turns the owner's and the target's animations toward each other and runs a timer.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CAttackComponent.h"
#include "game/CCharClassScorpiusAIComponent.h"
#include "game/CGrabComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU1501.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkVec3 {
    f32 x, y, z;
};

static inline UnkVec3 operator-(const UnkVec3& a, const UnkVec3& b) { return UnkVec3{a.x - b.x, a.y - b.y, a.z - b.z}; }

struct UnkScorpiusTunables_l69554c {
    u8 pad_0x0[0x5c];
    s32 mode;  // 0x5c
};

// The collider looked up by name; slot 0x60 takes a state value.
struct UnkScorpiusCollider_l69554c {
    void** vtable;
    void vf_0x60(s32 value) {
        reinterpret_cast<void (*)(UnkScorpiusCollider_l69554c*, s32)>(vtable[0x60 / 8])(this, value);
    }
};

struct UnkScorpiusAttack_l69554c : UnkReflObject {
    bool vf_0xe8() {
        void** vtable = *reinterpret_cast<void***>(this);
        return reinterpret_cast<bool (*)(UnkScorpiusAttack_l69554c*)>(vtable[0xe8 / 8])(this);
    }
};

struct UnkScorpiusPlayer_l69554c {
    u8 pad_0x0[0x1a0];
    UnkScorpiusAttack_l69554c* attack;  // 0x1a0
};

struct UnkScorpiusMovement_l69554c : UnkReflObject {
    u8 pad_0x8[0xf1];
    bool field_0xf9;
};

struct UnkScorpiusActor_l69554c {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x188];
    UnkScorpiusMovement_l69554c* movement;  // 0x200
    u8 pad_0x208[0x18];
    UnkAnimationComponent* anim;  // 0x220
    u8 pad_0x228[0x30];
    void* attack;  // 0x258
    void* grab;    // 0x260
    u8 pad_0x268[0x10];
    UnkScorpiusPlayer_l69554c* player;  // 0x278
    u8 pad_0x280[0x20];
    UnkCollisionContainer* collision;  // 0x2a0
};
static_assert(__builtin_offsetof(UnkScorpiusActor_l69554c, movement) == 0x200, "");
static_assert(__builtin_offsetof(UnkScorpiusActor_l69554c, attack) == 0x258, "");
static_assert(__builtin_offsetof(UnkScorpiusActor_l69554c, collision) == 0x2a0, "");

struct UnkScorpiusAI_l69554c {
    u8 pad_0x0[0x30];
    UnkScorpiusActor_l69554c* owner;  // 0x30
    u8 pad_0x38[0x1108];
    u32 phase;  // 0x1140
    u8 pad_0x1144[0x13ae4];
    bool field_0x14c28;
    u8 pad_0x14c29[0x97];
    bool mask_up;  // 0x14cc0
    u8 pad_0x14cc1[0xf];
    bool field_0x14cd0;
    u8 pad_0x14cd1[0x3];
    f32 mask_time;  // 0x14cd4
    u8 pad_0x14cd8[0x24];
    f32 timer;  // 0x14cfc
};
static_assert(__builtin_offsetof(UnkScorpiusAI_l69554c, phase) == 0x1140, "");
static_assert(__builtin_offsetof(UnkScorpiusAI_l69554c, field_0x14c28) == 0x14c28, "");
static_assert(__builtin_offsetof(UnkScorpiusAI_l69554c, mask_up) == 0x14cc0, "");
static_assert(__builtin_offsetof(UnkScorpiusAI_l69554c, mask_time) == 0x14cd4, "");
static_assert(__builtin_offsetof(UnkScorpiusAI_l69554c, timer) == 0x14cfc, "");

struct UnkBossAI_l3ae15c;
void fn_71003ae15c(UnkBossAI_l3ae15c* self, f32 dt);
struct UnkScorpiusAI_b59a4;
void fn_7100695f64(UnkScorpiusAI_b59a4* self);
void fn_7100695900(UnkScorpiusAI_l69554c* self);
void fn_7100695d78(UnkScorpiusAI_l69554c* self, f32 dt);
void fn_71006935a4(UnkScorpiusAI_l69554c* self, s32 value, bool flag);
void fn_7100692c8c(UnkScorpiusAI_l69554c* self, s32 value, bool flag);
bool fn_7100696088(UnkScorpiusAI_l69554c* self);
extern base::global::CStrId g_unk_71727041f0 __attribute__((visibility("hidden")));

static inline UnkScorpiusActor_l69554c* TargetOf(UnkScorpiusActor_l69554c* actor) {
    return reinterpret_cast<UnkScorpiusActor_l69554c*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(actor)));
}

static inline UnkVec3 Pos(const UnkScorpiusActor_l69554c* actor) {
    return UnkVec3{actor->xform.m[3], actor->xform.m[7], actor->xform.m[11]};
}

static inline bool InRegeneratingMaskTrack(UnkScorpiusAI_l69554c* self) {
    UnkAnimationComponent* anim = self->owner->anim;
    return anim && fn_710117b348(anim, fn_7100c4b288());
}

static inline bool IsGrabbed(UnkScorpiusAI_l69554c* self) {
    UnkScorpiusActor_l69554c* owner = self->owner;
    return owner->grab && fn_710113d83c(owner->grab);
}

static inline bool FacesTarget(UnkScorpiusAI_l69554c* self) {
    UnkScorpiusActor_l69554c* target = TargetOf(self->owner);
    if (!target)
        return false;
    UnkScorpiusMovement_l69554c* movement = target->movement;
    if (!movement)
        return false;
    UnkScorpiusMovement_l69554c* samus =
        fn_710009d50c(movement->vf_0x0(), fn_71009dda1c()) ? movement : nullptr;
    if (movement->field_0xf9 && !(samus && fn_71009e6434(reinterpret_cast<UnkHelper*>(samus))))
        return false;
    UnkScorpiusActor_l69554c* owner = self->owner;
    UnkVec3 to = Pos(target) - Pos(owner);
    const UnkMatrix44& m = fn_710002614c(&owner->xform);
    return to.x * m.m[2] + to.y * m.m[6] + to.z * m.m[10] > 0.5f;
}

// Starts the mask step in phase 2 when the owner is not turning, not in an unfinished attack, not
// grabbed, and faces its target.
static inline void TryRaiseMask(UnkScorpiusAI_l69554c* self) {
    bool attacking = false;
    UnkScorpiusActor_l69554c* owner = self->owner;
    if (owner && owner->attack && owner->anim) {
        void* attack = owner->attack;
        UnkAnimationComponent* anim = owner->anim;
        if (fn_710117b348(anim, fn_7100d2b3a8()))
            return;
        if (fn_7101136308(attack) || fn_710117b348(anim, fn_7100c0d1d8())) {
            UnkScorpiusAttack_l69554c* current = self->owner->player->attack;
            if (current && fn_710009d50c(current->vf_0x0(), fn_710068629c()))
                attacking = !current->vf_0xe8();
        }
    }
    if (self->phase != 2 || attacking)
        return;
    if (IsGrabbed(self) || !FacesTarget(self))
        return;
    fn_7100695f64(reinterpret_cast<UnkScorpiusAI_b59a4*>(self));
}

void fn_710069554c(UnkScorpiusAI_l69554c* self, f32 dt) {
    fn_71003ae15c(reinterpret_cast<UnkBossAI_l3ae15c*>(self), dt);
    fn_7100695900(self);
    fn_7100695d78(self, dt);
    switch (reinterpret_cast<const UnkScorpiusTunables_l69554c*>(&g_unk_7172703e88)->mode) {
    case 2:
        fn_7100692c8c(self, 0, true);
        break;
    case 1:
        fn_71006935a4(self, 0, true);
        break;
    case 0:
        if (self->field_0x14c28)
            fn_71006935a4(self, 0, true);
        else
            fn_7100692c8c(self, 0, true);
        break;
    }

    if (self->mask_up && InRegeneratingMaskTrack(self)) {
        if (fn_7100696088(self))
            self->mask_time += dt;
    } else if (self->mask_up) {
        TryRaiseMask(self);
    }

    if (self->field_0x14cd0) {
        UnkScorpiusActor_l69554c* owner = self->owner;
        if (owner->collision && owner->anim)
            reinterpret_cast<UnkScorpiusCollider_l69554c*>(fn_7100864194(owner->collision, &g_unk_71727041f0))
                ->vf_0x60(0x180);
    }

    UnkActorTarget* own_anim = reinterpret_cast<UnkActorTarget*>(self->owner->anim);
    UnkActorTarget* target_anim = reinterpret_cast<UnkActorTarget*>(TargetOf(self->owner)->anim);
    bool right = Pos(self->owner).x > Pos(TargetOf(self->owner)).x;
    u32 turn = right ? 5 : 4;
    u32 face = right ? 4 : 5;
    fn_7101178e08(own_anim, turn, false, false, -1.0f);
    fn_710117662c(own_anim, face, 1, false, false, -1.0f);
    fn_7101178e08(target_anim, turn, false, false, -1.0f);
    fn_710117662c(target_anim, face, 1, false, false, -1.0f);

    if (self->timer >= 0.0f)
        self->timer += dt;
}
