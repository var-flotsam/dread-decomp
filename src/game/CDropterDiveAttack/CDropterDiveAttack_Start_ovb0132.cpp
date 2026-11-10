// CDropterDiveAttack's unit (tools/tu_map.py unit 778): the dive's start (fn_71004e997c). After the base
// start it aims at the target's "Chest001" collider (or the target itself when it has +0x210 or its helper
// is ready), rounds the point (fn_710007b4b0), records the owner's position at +0x40 and the flat
// direction to the point at +0x58, and enters state 1 (fn_71004e9b20). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

using base::global::CStrId;

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
bool fn_71009e12d0(void*);
struct UnkVec3fR;
void fn_710007b4b0(UnkVec3fR* v);

struct UnkDropterVec3_ovb0132 {
    f32 x, y, z;
};

// A transform as the collision object's vf_0x70 returns it: the translation column at +0xc, +0x1c, +0x2c.
struct UnkDropterMatrix_ovb0132 {
    u8 pad_0x0[0xc];
    f32 x;
    u8 pad_0x10[0xc];
    f32 y;
    u8 pad_0x20[0xc];
    f32 z;
};

static inline UnkDropterMatrix_ovb0132* Transform(UnkCollisionObject* object) {
    return (*reinterpret_cast<UnkDropterMatrix_ovb0132* (***)(UnkCollisionObject*)>(object))[0x70 / 8](object);
}

struct UnkDropterActorView_ovb0132 {
    u8 pad_0x0[0x210];
    void* field_0x210;
    u8 pad_0x218[0x88];
    UnkCollisionContainer* collision;  // +0x2a0
};

struct UnkDropterDive_ovb0132 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // +0x8
    u8 pad_0x10[0x30];
    UnkDropterVec3_ovb0132 start;  // +0x40
    u8 pad_0x4c[0xc];
    UnkDropterVec3_ovb0132 dir;  // +0x58
};

void fn_71004e9b20(UnkDropterDive_ovb0132* self, bool a, bool b);

void fn_71004e997c(UnkDropterDive_ovb0132* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    CStrId name("Chest001", true);
    UnkActor* target = fn_7100d7c990(self->owner);
    UnkDropterActorView_ovb0132* view = reinterpret_cast<UnkDropterActorView_ovb0132*>(target);
    UnkCollisionObject* chest = fn_71008640a4(view->collision, &name);
    const f32* px;
    const f32* py;
    const f32* pz;
    if (view->field_0x210 || fn_71009e12d0(target->field_0x208)) {
        px = &target->field_0x44;
        py = &target->field_0x54;
        pz = &target->field_0x64;
    } else {
        UnkDropterMatrix_ovb0132* m = Transform(chest);
        px = &m->x;
        py = &m->y;
        pz = &m->z;
    }
    f32 y = *py;
    f32 x = *px;
    f32 z = *pz;
    UnkDropterVec3_ovb0132 pos = {x, y, z};
    fn_710007b4b0(reinterpret_cast<UnkVec3fR*>(&pos));
    UnkActor* owner = self->owner;
    f32 ox = owner->field_0x44;
    f32 oy = owner->field_0x54;
    UnkDropterVec3_ovb0132 d;
    d.x = pos.x - ox;
    d.y = pos.y - oy;
    d.z = 0.0f;
    f32 inv = 1.0f / (__builtin_sqrtf(d.x * d.x + d.y * d.y + d.z * d.z) + 0.0001f);
    d.x *= inv;
    d.y *= inv;
    d.z *= inv;
    owner = self->owner;
    f32 sx = owner->field_0x44;
    f32 sy = owner->field_0x54;
    f32 sz = owner->field_0x64;
    self->start.x = sx;
    self->start.y = sy;
    self->start.z = sz;
    self->dir = d;
    fn_71004e9b20(self, false, true);
}
