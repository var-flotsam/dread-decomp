// CMissileMovement unit (tools/tu_map.py unit 1618): when the sound player's object (fn_7100ed77a0) has a
// collision component with a record for it, takes the transform of the bone the record names from the
// component owner's model, moves it to the missile's position, offsets it and hands the result to
// `target` (slot 0x168). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CCollisionComponent.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU253.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

// As src/game/CBillboardCollisionComponent/CBillboardCollisionComponent_Remove_ovb0137.cpp defines it.
struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

u32 fn_71001c14fc(char* self, const void* name);
UnkMatrix44 fn_71000793c0(const UnkVec3f& translation, const UnkVec3f& angle);

struct UnkMissileSoundObject_laba6d4 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // 0x10
};

struct UnkMissileModelData_laba6d4 {
    u8 pad_0x0[0x138];
    char* resource;  // 0x138
};

struct UnkMissileModel_laba6d4 {
    u8 pad_0x0[0x50];
    UnkMissileModelData_laba6d4* data;  // 0x50
};

struct UnkMissileOwner_laba6d4 {
    u8 pad_0x0[0x228];
    UnkMissileModel_laba6d4* model;  // 0x228
};

struct UnkMissileCollision_laba6d4 {
    u8 pad_0x0[0x30];
    UnkMissileOwner_laba6d4* owner;  // 0x30
};

struct UnkMissileNameText_laba6d4 {
    u8 pad_0x0[0x30];
    const char* text;  // 0x30
};

struct UnkMissileTarget_laba6d4 {
    void** vtable;
    void vf_0x168(UnkMissileModelData_laba6d4* data, const UnkMatrix44* m, u32 flag, u32 bone) {
        reinterpret_cast<void (*)(UnkMissileTarget_laba6d4*, UnkMissileModelData_laba6d4*, const UnkMatrix44*, u32,
                                  u32)>(vtable[0x168 / 8])(this, data, m, flag, bone);
    }
};

struct UnkMissileMovement_laba6d4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

static inline const char* Text(const base::UnkStrIdMember& id) {
    return reinterpret_cast<const UnkMissileNameText_laba6d4*>(id.data)->text;
}

bool fn_7100aba6d4(UnkMissileMovement_laba6d4* self, UnkMissileTarget_laba6d4* target, const UnkVec3f* offset,
                   const UnkVec3f* angle) {
    UnkMissileSoundObject_laba6d4* object =
        static_cast<UnkMissileSoundObject_laba6d4*>(fn_7100ed77a0(g_unk_71726af0b0));
    if (!object)
        return false;
    UnkReflObject* component = object->component;
    if (!component || !fn_710009d50c(component->vf_0x0(), fn_710085fc44()))
        return false;
    UnkCollisionRecord* record = fn_710086411c(reinterpret_cast<UnkCollisionContainer*>(component),
                                               reinterpret_cast<UnkCollisionObject*>(object));
    if (!record)
        return false;
    UnkMissileModel_laba6d4* model = reinterpret_cast<UnkMissileCollision_laba6d4*>(component)->owner->model;
    if (!model)
        return false;
    UnkMissileModelData_laba6d4* data = model->data;
    if (!data)
        return false;

    UnkMatrix44 bone = {};
    bool found;
    bone.m[5] = 1.0f;
    bone.m[15] = 1.0f;
    bone.m[10] = 1.0f;
    bone.m[0] = 1.0f;
    {
        const char* name = Text(record->field_0x10);
        found = fn_71001c1694(data->resource, &name, bone.m);
    }
    if (!found)
        return false;

    UnkActor* owner = self->owner;
    UnkMatrix44_7100078c9c move = fn_7100078cd8(UnkVec3f(owner->field_0x44, owner->field_0x54, owner->field_0x64));
    UnkMatrix44 world = fn_71000127c8(fn_7100078314(&bone), reinterpret_cast<const UnkMatrix44&>(move));
    // The bone's index by name, and the translation of the bone's transform at the missile.
    f32 x, y, z;
    u32 index;
    {
        x = world.m[3];
        z = world.m[11];
        y = world.m[7];
        const char* name = Text(record->field_0x10);
        index = fn_71001c14fc(data->resource, &name);
    }
    UnkVec3f pos(x + offset->x, y + offset->y, z + offset->z);
    UnkMatrix44 result = fn_71000793c0(pos, *angle);
    target->vf_0x168(data, &result, 0, index);
    return false;
}
