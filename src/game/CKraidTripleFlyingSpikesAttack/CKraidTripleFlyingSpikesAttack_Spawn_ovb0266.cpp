// CKraidTripleFlyingSpikesAttack's spawn of one spike: loads the actor definition `id` through the actordef factory
// (as src/game/CKraidFlyingSpikesAttack/CKraidFlyingSpikesAttack_Spawn_ovb0118.cpp), and for an entity registers
// it with the sub-area points, hands it the Kraid AI's value (+0x14d30, less 30) at +0x25c of its component,
// puts it at `position` plus the platform's offset (+0x7c) and turns it a quarter turn about y, then flags it
// (+0x3c0). Returns the entity, or null. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkActorDefFactory;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeType_b52;
UnkPowerUpLifeEntry_b52* fn_710119438c(UnkActorDefFactory* factory, const char* path, CStrId* id, const UnkPowerUpLifeType_b52& type,
                                       void* arg);
void fn_71011a2770(UnkGameManagerSub_0x608* points, void* object);

struct UnkKraidName_b0266 {
    u8 unk_0x0[0x10];
    char text[0x34];
    union {
        u32 use_count;  // 0x44
    };
};

struct alignas(8) UnkKraidDefType_b0266 {
    u32 id;
};

struct UnkSpikeComponent_b0266 {
    u8 pad_0x0[0x10];
    UnkReflObject* platform;  // 0x10
    u8 pad_0x18[0x244];
    f32 field_0x25c;
};

struct UnkSpikeActor_b0266 : UnkReflObject {
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x31c];
    u8 field_0x3b0;
    u8 pad_0x3b1[0xf];
    bool field_0x3c0;

    void vf_0x118(UnkXform* x, s32* mode) {
        reinterpret_cast<void (*)(UnkSpikeActor_b0266*, UnkXform*, s32*)>((*reinterpret_cast<void***>(this))[0x118 / 8])(this, x, mode);
    }
};

struct UnkKraidOwner_b0266 {
    u8 unk_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

struct UnkKraidTripleAttack_b0266 {
    u8 unk_0x0[0x8];
    UnkKraidOwner_b0266* owner;  // 0x8
};

UnkSpikeComponent_b0266* fn_71005f55a4(UnkSpikeActor_b0266* actor);
void fn_710060887c(UnkSpikeComponent_b0266* component);

UnkSpikeActor_b0266* fn_71005f8148(UnkKraidTripleAttack_b0266* self, const UnkVec3f* position, CStrId* id) {
    UnkSpikeActor_b0266* actor;
    {
        UnkKraidDefType_b0266 type;
        UnkActorDefFactory* factory = g_unk_71726a0220;
        u32 type_id = g_unk_7172a0ba48;
        UnkKraidName_b0266* name = reinterpret_cast<UnkKraidName_b0266*>(id->data);
        ++name->use_count;
        const char* text = reinterpret_cast<UnkKraidName_b0266*>(id->data)->text;
        type.id = type_id;
        actor = reinterpret_cast<UnkSpikeActor_b0266*>(
            fn_710119438c(factory, text, id, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    }
    if (!actor)
        return nullptr;
    if (!fn_710009d50c(actor->vf_0x0(), fn_7100d70c20()))
        return nullptr;
    if (UnkGameManagerSub_0x608* points = g_unk_7101d47310->field_0x608)
        fn_71011a2770(points, actor);
    UnkReflObject* ai = self->owner->ai;
    UnkReflObject* kraid = ai && fn_710009d50c(ai->vf_0x0(), fn_71005fac74()) ? ai : nullptr;
    UnkSpikeComponent_b0266* component = fn_71005f55a4(actor);
    if (kraid && component)
        component->field_0x25c = *reinterpret_cast<f32*>(reinterpret_cast<char*>(kraid) + 0x14d30) + -30.0f;
    UnkReflObject* platform_object = component->platform;
    UnkReflObject* platform =
        fn_710009d50c(platform_object->vf_0x0(), fn_71006039e4()) ? platform_object : nullptr;
    f32 offset = *reinterpret_cast<f32*>(reinterpret_cast<char*>(platform) + 0x7c);
    f32 x = offset + position->x;
    f32 y = position->y + 0.0f;
    f32 z = position->z + 0.0f;
    actor->xform.m[0][3] = x;
    actor->xform.m[1][3] = y;
    actor->xform.m[2][3] = z;
    {
        s32 mode = 1;
        actor->vf_0x118(&actor->xform, &mode);
    }
    // untyped: the actor's flags are reloaded afterwards
    *reinterpret_cast<u8*>(&actor->field_0x3b0) = 1;
    actor->xform.SetAngle(UnkVec3f(0.0f, -1.57079637f, 0.0f));
    {
        s32 mode = 2;
        actor->vf_0x118(&actor->xform, &mode);
    }
    actor->field_0x3c0 = true;
    fn_710060887c(component);
    return actor;
}
