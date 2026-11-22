// CDredhedDiveAttack's unit: starts the dive. Resets the attack's state, then, when the target has its
// object at +0x210 and its flag at +0xf9 is clear, plays the owner's animation one way and enables the
// collider; otherwise the other way with the collider's flag 0x100000. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkAttack.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkShakernautId.h"
#include "types.h"
#include "../../../src/game_s7/UnkPropBagS7.h"

using base::global::CStrId;

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726da328;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726da330;

struct UnkDredhedDiveCollider {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60(u32 flags);
};

struct UnkDredhedDiveTargetState {
    u8 pad_0x0[0xf9];
    u8 field_0xf9;
};

struct UnkDredhedDiveTarget {
    u8 pad_0x0[0x1f0];
    UnkDredhedDiveTargetState* field_0x1f0;
    u8 pad_0x1f8[0x18];
    void* field_0x210;
};

struct UnkDredhedDiveOwner {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
    u8 pad_0x228[0x78];
    UnkCollisionContainer* collision;
};

struct UnkDredhedDiveAttackStart {
    void** vtable;
    UnkDredhedDiveOwner* owner;
    u8 pad_0x10[0x24];
    bool field_0x34;
    u8 pad_0x35[3];
    f32 field_0x38;
    u32 field_0x3c;
    u8 field_0x40;
    u8 pad_0x41[3];
    f32 field_0x44;
    u32 field_0x48;
    u8 pad_0x4c[4];
    u8 field_0x50;
    u8 pad_0x51[3];
    u32 field_0x54;
    u8 field_0x58;
};

void fn_71004e4a38(UnkDredhedDiveAttackStart* self, u32 value);

void fn_71004e490c(UnkDredhedDiveAttackStart* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->field_0x38 = -1.0f;
    self->field_0x3c = 0;
    self->field_0x48 = 0;
    self->field_0x40 = 0;
    self->field_0x50 = 0;
    self->field_0x58 = 0;
    UnkDredhedDiveTarget* target = reinterpret_cast<UnkDredhedDiveTarget*>(
        fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)));
    u8 blocked = target->field_0x1f0->field_0xf9;
    void* object = target->field_0x210;
    self->field_0x54 = 0;
    bool enabled = blocked == 0 && object != nullptr;
    self->field_0x34 = enabled;
    self->field_0x44 = enabled ? 0.0f : 1.0f;
    UnkShakernautCtrl_b53* animation = reinterpret_cast<UnkShakernautCtrl_b53*>(self->owner->animation);
    UnkCollisionContainer* collision = self->owner->collision;
    if (object != nullptr && blocked == 0) {
        UnkShakernautId_b53 id;
        id.value = 0x200;
        fn_7101178564(animation, &id, true, false);
        reinterpret_cast<UnkDredhedDiveCollider*>(fn_7100864194(collision, &g_unk_71726da328))->vf_0x60(0);
    } else {
        UnkShakernautId_b53 id;
        id.value = 0x200;
        fn_71011785a8(animation, &id, true, false);
        reinterpret_cast<UnkDredhedDiveCollider*>(fn_7100864194(collision, &g_unk_71726da328))->vf_0x60(0x100000);
    }
    unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(self->owner->animation->field_0x80),
                                g_unk_71726da330, self->field_0x44);
    fn_71004e4a38(self, 0);
}
