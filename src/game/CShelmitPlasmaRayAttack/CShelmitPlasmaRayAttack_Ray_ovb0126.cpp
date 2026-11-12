// CShelmitPlasmaRayAttack's unit (tools/tu_map.py): fn_71006b4594 places the ray. With the collider
// fn_71006b6100 finds, the owner's (+0x8) world (+0x280), the collider's shape (+0x158) and its flag at +0x6a
// set, the shape's box (vf_0x28) gives the ray's origin (the middle of the box bottom); the effect
// 0x132e4760c37d0e46 that the world holds is scaled by the distance between the box's top and bottom / 100
// (vf_0x20), and the objects at +0x58, +0x60 and +0x68 are moved to the origin (vf_0x10). Names and
// layouts are placeholders.
#include "game/CFXComponent.h"
#include "types.h"

struct UnkWorld;
struct UnkPart;
// As src/game/CAbilityEnergyWave/fn_7100300bd0.cpp declares it.

struct UnkRayVec_ovb0126 {
    f32 x, y, z;
};

struct UnkRayBox_ovb0126 {
    UnkRayVec_ovb0126 min;
    UnkRayVec_ovb0126 max;
};

struct UnkRayShape_ovb0126 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkRayBox_ovb0126 vf_0x28(void* data);
    u8 field_0x8[8];
};

struct UnkRayCollider_ovb0126 {
    u8 pad_0x0[0x6a];
    bool field_0x6a;
    u8 pad_0x6b[0x158 - 0x6b];
    UnkRayShape_ovb0126* shape;
};

struct UnkRayObject_ovb0126 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(const UnkRayVec_ovb0126* position);
    virtual void vf_0x18();
    virtual void vf_0x20(const UnkRayVec_ovb0126* scale);
};

struct UnkRayOwner_ovb0126 {
    u8 pad_0x0[0x280];
    UnkWorld* world;
};

struct UnkRayAttack_ovb0126 {
    u8 pad_0x0[0x8];
    UnkRayOwner_ovb0126* owner;
    u8 pad_0x10[0x58 - 0x10];
    UnkRayObject_ovb0126* field_0x58;
    UnkRayObject_ovb0126* field_0x60;
    UnkRayObject_ovb0126* field_0x68;
};

UnkRayCollider_ovb0126* fn_71006b6100(UnkRayAttack_ovb0126* self);

void fn_71006b4594(UnkRayAttack_ovb0126* self) {
    UnkRayCollider_ovb0126* collider = fn_71006b6100(self);
    UnkWorld* world = self->owner ? self->owner->world : nullptr;
    asm("" : "+r"(world));
    if (!collider || !world)
        return;
    UnkRayShape_ovb0126* shape = collider->shape;
    if (!shape || !collider->field_0x6a)
        return;
    UnkRayBox_ovb0126 box = shape->vf_0x28(shape->field_0x8);
    f32 top = box.max.y;
    UnkRayVec_ovb0126 position;
    position.x = (box.min.x + box.max.x) * 0.5f;
    position.y = box.min.y;
    position.z = 0.0f;
    if (auto* effect = reinterpret_cast<UnkRayObject_ovb0126*>(fn_7101152b90(world, 0x132e4760c37d0e46ULL))) {
        UnkRayVec_ovb0126 scale;
        scale.x = __builtin_fabsf(top - position.y) / 100.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;
        effect->vf_0x20(&scale);
    }
    if (self->field_0x58)
        self->field_0x58->vf_0x10(&position);
    if (self->field_0x60)
        self->field_0x60->vf_0x10(&position);
    if (self->field_0x68)
        self->field_0x68->vf_0x10(&position);
}
