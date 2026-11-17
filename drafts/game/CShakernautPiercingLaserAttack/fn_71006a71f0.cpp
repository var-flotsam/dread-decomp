// CShakernautPiercingLaserAttack's unit: the attack's start. Records the side the AI component faces, the
// grapple answer, the start position and the owner's translation, picks the step from the animation's id,
// finds the collider and hooks the callback up to the animation. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
u32 fn_71006a9a5c();

extern const u64 g_unk_710164a660;
extern CStrId g_unk_7172705c70[2] __attribute__((visibility("hidden")));
extern CStrId g_unk_7172705cf0 __attribute__((visibility("hidden")));

struct UnkShakernautFacing_l6a71f0 {
    f32 x;
    f32 y;
};

struct UnkShakernautAI_l6a71f0 : UnkReflObject {
    u8 pad_0x8[0x14810 - 0x8];
    UnkShakernautFacing_l6a71f0 facing;  // 0x14810
};

struct UnkShakernautOwner_l6a71f0 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* anim;      // 0x220
    void* model_updater;      // 0x228
    u8 pad_0x230[0x10];
    UnkReflObject* ai;        // 0x240
    u8 pad_0x248[0x58];
    UnkReflObject* collision;  // 0x2a0
};

struct UnkShakernautLaserLink_l6a71f0 {
    u8 pad_0x0[0x8];
    void* context;
    u32 (*callback)();
};

struct UnkShakernautLaserAttack_l6a71f0 {
    u8 pad_0x0[0x8];
    UnkShakernautOwner_l6a71f0* owner;
    u8 pad_0x10[0x20];
    u32 step;  // 0x30
    u8 pad_0x34[0x4];
    UnkShakernautLaserLink_l6a71f0 link;  // 0x38
    UnkWeakHandle* handle;                // 0x50
    UnkWeakTarget* anim;                  // 0x58
    u64 field_0x60;
    u64 field_0x68;
    u8 pad_0x70[0x4];
    u8 grapple_answer[0x18];  // 0x74
    CVector3D facing;         // 0x8c
    u8 pad_0x98[0x4];
    CVector3D origin;         // 0x9c
    u64 field_0xa8;
    u32 side;  // 0xb0
    u8 pad_0xb4[0x4];
    UnkCollisionObject* collider;  // 0xb8
};
static_assert(__builtin_offsetof(UnkShakernautLaserAttack_l6a71f0, collider) == 0xb8, "layout");

void fn_71006a71f0(UnkShakernautLaserAttack_l6a71f0* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->step = 0;
    UnkReflObject* ai = self->owner->ai;
    UnkShakernautAI_l6a71f0* shakernaut =
        fn_710009d50c(ai->vf_0x0(), fn_71006ad0a4()) ? static_cast<UnkShakernautAI_l6a71f0*>(ai) : nullptr;
    UnkShakernautFacing_l6a71f0* facing = &shakernaut->facing;
    self->side = facing->x + facing->y * 0.0f > 0.0f ? 2 : 1;
    fn_710117364c(self->owner->model_updater, &g_unk_710164a660,
                  reinterpret_cast<UnkGrappleAnswer_b50*>(self->grapple_answer));
    f32 facing_x = facing->x;
    f32 facing_y = facing->y;
    self->facing.x = facing_x;
    self->facing.y = facing_y;
    self->facing.z = 0.0f;
    self->anim = self->owner->anim;
    CStrId id(*fn_7101177ae8(reinterpret_cast<UnkAnimationComponent*>(self->anim), 0xf4061ff25a2163d6));
    const CStrId* ids = g_unk_7172705c70;
    bool known = (id.data == ids[0].data) | (id.data == ids[1].data);
    self->field_0x60 = 0;
    self->field_0x68 = 0;
    self->field_0xa8 = 0;
    self->step = known ? 2 : 1;
    UnkActor* actor = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    f32 origin_x = actor->field_0x44;
    f32 origin_y = actor->field_0x54;
    f32 origin_z = actor->field_0x64;
    self->origin.x = origin_x;
    self->origin.y = origin_y;
    self->origin.z = origin_z;
    UnkReflObject* collision = self->owner->collision;
    if (collision && !fn_710009d50c(collision->vf_0x0(), fn_710085fc44()))
        collision = nullptr;
    self->collider = fn_71008640a4(reinterpret_cast<UnkCollisionContainer*>(collision), &g_unk_7172705cf0);
    UnkShakernautLaserLink_l6a71f0* link = &self->link;
    link->context = self;
    link->callback = fn_71006a9a5c;
    UnkWeakTarget* track = self->anim;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (track) {
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
}
