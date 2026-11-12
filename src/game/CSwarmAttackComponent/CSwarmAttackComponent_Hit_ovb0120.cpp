// CSwarmAttackComponent's unit (tools/tu_map.py unit 1162): the hit handler (fn_71007465e0). The hit's
// collider must be a CCollisionComponent whose owner is another actor with a +0x1b8 object that is not
// busy (fn_7101161088), while the attacker's owner has its flag at +0x1b5 clear; an owner playing a
// CCharClassMeleeTrack track is skipped. Then the hit goes to vtable slot 0x198 and the attacker's
// effect (fn_71007466f4) fires through its slot 0x248. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CLifeComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkSwarmTarget_ovb0120 {
    u8 pad_0x0[0x1b5];
    bool field_0x1b5;
    u8 pad_0x1b6[2];
    void* field_0x1b8;  // tested with fn_7101161088
    u8 pad_0x1c0[0x60];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkSwarmCollision_ovb0120 : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkSwarmTarget_ovb0120* owner;  // 0x30
};

struct UnkSwarmCollider_ovb0120 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // 0x10
};

struct UnkSwarmHit_ovb0120 {
    u8 pad_0x0[0x10];
    UnkSwarmCollider_ovb0120* collider;  // 0x10
};

struct UnkSwarmRef_ovb0120 {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};

struct UnkSwarmMessage_ovb0120 {
    u8 pad_0x0[0x10];
    UnkSwarmHit_ovb0120* hit;  // 0x10
    UnkSwarmRef_ovb0120* source;  // 0x18
};

struct UnkSwarmEffect_ovb0120 {
    virtual void vf_0x0();
    void Fire() { reinterpret_cast<void (***)(UnkSwarmEffect_ovb0120*)>(this)[0][0x248 / 8](this); }
};


UnkSwarmEffect_ovb0120* fn_71007466f4(UnkSwarmTarget_ovb0120* owner);

struct UnkSwarmAttack_ovb0120 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkSwarmTarget_ovb0120* owner;  // 0x30

    using OnHitFn = void (*)(UnkSwarmAttack_ovb0120*, UnkSwarmTarget_ovb0120*, UnkSwarmHit_ovb0120**, void*, bool);
};

static inline bool CanHit(const UnkSwarmTarget_ovb0120& target, const UnkSwarmTarget_ovb0120& owner) {
    return target.field_0x1b8 && !owner.field_0x1b5;
}

bool fn_71007465e0(UnkSwarmAttack_ovb0120* self, UnkSwarmMessage_ovb0120* msg) {
    UnkReflObject* component = msg->hit->collider->component;
    if (!component || !fn_710009d50c(component->vf_0x0(), fn_710085fc44()))
        return false;
    UnkSwarmTarget_ovb0120* target = static_cast<UnkSwarmCollision_ovb0120*>(component)->owner;
    if (!target || target == self->owner)
        return false;
    if (!CanHit(*target, *self->owner))
        return false;
    UnkAnimationComponent* animation = target->animation;
    bool busy = fn_7101161088(target->field_0x1b8);
    if (!animation || busy)
        return false;
    if (fn_710117b348(animation, fn_7100d1ab58()))
        return false;
    void* source = msg->source ? msg->source->object : nullptr;
    reinterpret_cast<UnkSwarmAttack_ovb0120::OnHitFn>(self->vtable[0x198 / 8])(self, target, &msg->hit, source, false);
    if (UnkSwarmEffect_ovb0120* effect = fn_71007466f4(self->owner))
        effect->Fire();
    return true;
}
