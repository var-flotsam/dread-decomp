// CShelmitPlasmaRayAttack's unit (tools/tu_map.py): ends the ray. When the owner (+0x8) has its collision
// container (+0x2a0) and the unit's collider (g_unk_7172706938, fn_71008640a4) is in it: the owner's
// timeline (+0x2e8) gets g_unk_710164a700 set to the all-flags mask unless it already holds it
// (fn_7100a2cfe8); the effects at +0x58 and +0x60 that are active (fn_71002084b4 on +0x28) are switched off
// with fn_7100146dac and the one at +0x68 with fn_7100146e60; with a world (+0x280 of the owner) and a
// shape (+0x158 of the collider), the effect 0x81576b5cecf37e8b that is live in the world is moved to the
// middle of the shape's box bottom (vf_0x10) and scaled by the box height / 100 (vf_0x20); then the
// collider is removed (fn_7100863e04) and the flag at +0x72 cleared. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern char g_unk_7172706938[] __attribute__((visibility("hidden")));

struct UnkPlasmaRayKey_b60a4;
extern UnkPlasmaRayKey_b60a4 g_unk_710164a700;  // reached through the GOT

// As src/game/UnkUnit285/UnkUnit285_UpdateActive_b46.cpp defines it.
struct UnkUnit285_b46a1;
bool fn_71002084b4(UnkUnit285_b46a1* self);
// As src/game/behaviortree/CParticleEnableEmission/CParticleEnableEmission_Start_b49.cpp declares it.
void fn_7100146dac(UnkSwitchObject* self, bool enable);
// As src/game/CAbilityEnergyWave/fn_7100300bd0.cpp declares them.
struct UnkWorld;
struct UnkPart;
UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);
bool fn_710114e668(UnkWorld* world, u8 id);
void fn_7100863e04(UnkCollisionContainer* self, UnkCollisionObject* object);

struct UnkPlasmaRayVec_b60a4 {
    f32 x, y, z;
};

struct UnkPlasmaRayBox_b60a4 {
    UnkPlasmaRayVec_b60a4 min;
    UnkPlasmaRayVec_b60a4 max;
};

struct UnkPlasmaRayShape_b60a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkPlasmaRayBox_b60a4 vf_0x28(void* data);
    u8 field_0x8[8];
};

struct UnkPlasmaRayCollider_b60a4 {
    u8 pad_0x0[0x158];
    UnkPlasmaRayShape_b60a4* shape;
};

struct UnkPlasmaRayEffect_b60a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(const UnkPlasmaRayVec_b60a4* position);
    virtual void vf_0x18();
    virtual void vf_0x20(const UnkPlasmaRayVec_b60a4* scale);
    u8 pad_0x8[3];
    u8 id;
};

struct UnkPlasmaRayOwner_b60a4 {
    u8 pad_0x0[0x280];
    UnkWorld* world;
    u8 pad_0x288[0x2a0 - 0x288];
    UnkCollisionContainer* collision;
    u8 pad_0x2a8[0x2e8 - 0x2a8];
    UnkTimelineComponent* timeline;
};

struct UnkPlasmaRayAttack_b60a4 {
    u8 pad_0x0[0x8];
    UnkPlasmaRayOwner_b60a4* owner;
    u8 pad_0x10[0x58 - 0x10];
    UnkSwitchObject* field_0x58;
    UnkSwitchObject* field_0x60;
    UnkSwitchObject* field_0x68;
    u8 pad_0x70[2];
    bool field_0x72;
};

static inline bool Active_b60a4(UnkSwitchObject* p) {
    return fn_71002084b4(reinterpret_cast<UnkUnit285_b46a1*>(p->field_0x28));
}

void fn_71006b4900(UnkPlasmaRayAttack_b60a4* self) {
    UnkPlasmaRayOwner_b60a4* owner = self->owner;
    if (!owner || !owner->collision)
        return;
    UnkTimelineComponent* timeline = owner->timeline;
    UnkWorld* world = owner->world;
    UnkCollisionObject* collider = fn_71008640a4(
        owner->collision, reinterpret_cast<const base::global::CStrId*>(g_unk_7172706938));
    if (!collider)
        return;
    if (timeline && !fn_7100a2cfe8(timeline, reinterpret_cast<const u64*>(&g_unk_710164a700))) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a700), &mask.id, false, -1.0f);
    }
    if (self->field_0x58 && Active_b60a4(self->field_0x58))
        fn_7100146dac(self->field_0x58, false);
    if (self->field_0x60 && Active_b60a4(self->field_0x60))
        fn_7100146dac(self->field_0x60, false);
    if (self->field_0x68 && Active_b60a4(self->field_0x68))
        fn_7100146e60(self->field_0x68, false);
    if (world) {
        if (UnkPlasmaRayShape_b60a4* shape = reinterpret_cast<UnkPlasmaRayCollider_b60a4*>(collider)->shape) {
            UnkPlasmaRayBox_b60a4 box = shape->vf_0x28(shape->field_0x8);
            UnkPlasmaRayVec_b60a4 position;
            position.x = (box.min.x + box.max.x) * 0.5f;
            position.y = box.min.y;
            position.z = 0.0f;
            f32 height = __builtin_fabsf(box.max.y - box.min.y);
            auto* effect = reinterpret_cast<UnkPlasmaRayEffect_b60a4*>(fn_7101152b90(world, 0x81576b5cecf37e8bULL));
            if (effect && fn_710114e668(world, effect->id)) {
                effect->vf_0x10(&position);
                UnkPlasmaRayVec_b60a4 scale;
                scale.x = height / 100.0f;
                scale.y = 1.0f;
                scale.z = 1.0f;
                effect->vf_0x20(&scale);
            }
        }
    }
    if (UnkPlasmaRayOwner_b60a4* o = self->owner) {
        if (UnkCollisionContainer* collision = o->collision) {
            if (UnkCollisionObject* c = fn_71008640a4(
                    collision, reinterpret_cast<const base::global::CStrId*>(g_unk_7172706938)))
                fn_7100863e04(collision, c);
        }
    }
    self->field_0x72 = false;
}
