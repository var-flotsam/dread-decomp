// CKraidBouncingCreaturesAttack unit (unit 914): launches the next creature of the current pattern. When the
// pattern's slot holds an entry, loads one of the two creature actor definitions, places it at Kraid's
// launch point, and gives its movement component the launch direction (the entry's angle, mirrored to
// Kraid's facing), speed and timings; then moves on to the next slot. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBouncingCreaturesLaunchPattern.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkKraidBouncingList_b57a4.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkActorDefFactory;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeType_b52;

UnkVec3f RotateVec3AboutZ(const UnkVec3f& v, f32 angle);

// The two creature actor definitions.
extern CStrId g_unk_71726f2bb8[2] __attribute__((visibility("hidden")));
// The launch point's name on Kraid's model.
extern const char g_unk_710164a148[];
// The creature's starting angle.
extern const UnkVec3f g_unk_710164a150 __attribute__((visibility("hidden")));

struct UnkKraidName_l5f31e4 {
    u8 unk_0x0[0x10];
    char text[0x34];
    u32 use_count;  // 0x44
};

struct alignas(8) UnkKraidDefType_l5f31e4 {
    u32 id;
};

// One launch of a pattern.
struct UnkKraidLaunch_l5f31e4 {
    u8 pad_0x0[0x8];
    f32 angle;     // +0x8, degrees
    f32 field_0xc;
    f32 field_0x10;
};

struct UnkKraidOwner_l5f31e4 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // +0x38
    u8 pad_0x94[0x228 - 0x94];
    UnkEntity* model;  // +0x228
};
static_assert(__builtin_offsetof(UnkKraidOwner_l5f31e4, model) == 0x228, "layout");

struct UnkKraidCreature_l5f31e4 {
    void** vtable;

    void SetPosition(const UnkVec3f* position, bool flag) {
        reinterpret_cast<void (*)(UnkKraidCreature_l5f31e4*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(
            this, position, flag);
    }
    void SetAngle(const UnkVec3f* angle, bool flag) {
        reinterpret_cast<void (*)(UnkKraidCreature_l5f31e4*, const UnkVec3f*, bool)>(vtable[0xd8 / 8])(
            this, angle, flag);
    }
};

struct UnkKraidCreatureMovement_l5f31e4 {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkReflObject* field_0x10;
    u8 pad_0x18[0x40 - 0x18];
    UnkVec3f direction;  // +0x40
    u8 pad_0x4c[0x7c - 0x4c];
    f32 field_0x7c;
    u8 pad_0x80[0x108 - 0x80];
    f32 speed;  // +0x108
    u8 pad_0x10c[0x570 - 0x10c];
    bool field_0x570;
    u8 pad_0x571[0x5d8 - 0x571];
    f32 field_0x5d8;

    void Launch(UnkKraidOwner_l5f31e4* owner) {
        reinterpret_cast<void (*)(UnkKraidCreatureMovement_l5f31e4*, UnkKraidOwner_l5f31e4*)>(
            vtable[0x290 / 8])(this, owner);
    }
    void Start() {
        reinterpret_cast<void (*)(UnkKraidCreatureMovement_l5f31e4*)>(vtable[0x2c0 / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkKraidCreatureMovement_l5f31e4, field_0x5d8) == 0x5d8, "layout");

UnkKraidCreatureMovement_l5f31e4* fn_71005f34a8(UnkKraidCreature_l5f31e4* creature);

struct UnkKraidBouncingSet_l5f31e4 {
    u8 pad_0x0[0xf8];
    UnkKraidBouncingList_b57a4* lists;  // +0xf8
};

struct UnkKraidBouncingAttack_l5f31e4 {
    u8 pad_0x0[0x8];
    UnkKraidOwner_l5f31e4* owner;  // +0x8
    u8 pad_0x10[0x20];
    UnkKraidBouncingSet_l5f31e4* set;  // +0x30
    u32 list;                          // +0x38
    u32 slot;                          // +0x3c
    u8 pad_0x40[0x4];
    bool second_creature;  // +0x44
};

void CKraidBouncingCreaturesAttack_LaunchNextPatternCreature(UnkKraidBouncingAttack_l5f31e4* self) {
    UnkKraidBouncingList_b57a4* lists = self->set->lists;
    u32 slot = self->slot;
    u32 index = self->list;
    auto* launch = static_cast<UnkKraidLaunch_l5f31e4*>(*fn_71005e2c08(&lists[index], slot));
    if (!launch)
        return;

    CStrId* id = self->second_creature ? &g_unk_71726f2bb8[1] : &g_unk_71726f2bb8[0];
    UnkActorDefFactory* factory = g_unk_71726a0220;
    UnkKraidName_l5f31e4* name = reinterpret_cast<UnkKraidName_l5f31e4*>(id->data);
    ++name->use_count;
    const char* path = name->text;
    UnkKraidDefType_l5f31e4 type;
    type.id = g_unk_7172a0ba48;
    auto* creature = reinterpret_cast<UnkKraidCreature_l5f31e4*>(fn_710119438c(
        factory, path, id, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    if (creature) {
        UnkKraidOwner_l5f31e4* owner = self->owner;
        UnkVec3f position = owner->xform.GetTranslation();
        UnkMatrix44 point = {};
        point.m[15] = 1.0f;
        point.m[10] = 1.0f;
        point.m[5] = 1.0f;
        point.m[0] = 1.0f;
        if (fn_71011735e8(owner->model, g_unk_710164a148, &point, 0))
            position = UnkVec3f(point.m[3], point.m[7], point.m[11]);

        f32 angle = launch->angle * 3.141592653589793 / 180.0;
        UnkVec3f dir = RotateVec3AboutZ(UnkVec3f(1.0f, 0.0f, 0.0f), angle);
        if (self->owner->xform.GetMatrix().m[2] < 0.0f)
            dir.x = -dir.x;
        f32 inv = InvLengthEps(dir);
        dir.x *= inv;
        dir.y *= inv;
        dir.z *= inv;

        creature->SetPosition(&position, true);
        creature->SetAngle(&g_unk_710164a150, true);
        UnkKraidCreatureMovement_l5f31e4* movement = fn_71005f34a8(creature);
        if (movement && movement->field_0x10 &&
            fn_710009d50c(movement->field_0x10->vf_0x0(), fn_7100602884())) {
            movement->field_0x570 = false;
            movement->field_0x7c = launch->field_0xc;
            movement->speed = 3700.0f;
            movement->direction = dir;
            movement->Launch(self->owner);
            movement->field_0x5d8 = launch->field_0x10;
            movement->Start();
        }
    }
    ++self->slot;
}
