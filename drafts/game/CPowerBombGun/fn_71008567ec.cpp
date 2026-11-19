// CPowerBombGun's unit (tools/tu_map.py, unit 1313): fires a power bomb. Once the gun's slot 0x48 allows
// it, the bomb's actor definition (the owner's tweak track, or the id table's "powerbomb") is spawned; an
// entity takes one bomb from the inventory, is placed where its bomb component says, tagged and charged
// from the gun's settings, the owner's script is told, and the gun keeps a weak handle to it.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CInventoryComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkInventory;
struct UnkActor;
struct UnkWeakTarget;
struct UnkPowerUpLifeType_b52;

// An actor definition's name as the factory reads it.
struct UnkPowerBombName_l8567ec {
    u8 unk_0x0[0x10];
    char text[0x34];
    union {
        u32 use_count;  // 0x44
    };
};

struct alignas(8) UnkPowerBombDefType_l8567ec {
    u32 id;
};

struct UnkPowerBombOwner_l8567ec {
    u8 pad_0x0[0x1e0];
    UnkScriptCaller* script;  // 0x1e0
    u8 pad_0x1e8[0x220 - 0x1e8];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x270 - 0x228];
    UnkInventory* inventory;  // 0x270
};

// The spawned bomb's actor.
struct UnkPowerBombActor_l8567ec : UnkReflObject {
    void SetPosition(const CVector3D* pos, bool flag) {
        reinterpret_cast<void (*)(UnkPowerBombActor_l8567ec*, const CVector3D*, bool)>(
            (*reinterpret_cast<void***>(this))[0xd0 / 8])(this, pos, flag);
    }
    void SetRotation(const CVector3D* rot, bool flag) {
        reinterpret_cast<void (*)(UnkPowerBombActor_l8567ec*, const CVector3D*, bool)>(
            (*reinterpret_cast<void***>(this))[0xd8 / 8])(this, rot, flag);
    }
};

// The bomb's own component.
struct UnkPowerBomb_l8567ec {
    void** vtable;
    u8 pad_0x8[0x348 - 0x8];
    u8 charge[0xb4];  // 0x348, the request fn_71008c1538 fills
    u32 field_0x3fc;
    u8 pad_0x400[8];
    f32 field_0x408;
    f32 field_0x40c;

    // vtable slot 0x2c8: where the bomb starts, for the owner.
    void Placement(UnkPowerBombOwner_l8567ec* owner, CVector3D* pos, CVector3D* rot) {
        reinterpret_cast<void (*)(UnkPowerBomb_l8567ec*, UnkPowerBombOwner_l8567ec*, CVector3D*, CVector3D*)>(
            vtable[0x2c8 / 8])(this, owner, pos, rot);
    }
    // vtable slot 0x290.
    void vf_0x290(UnkPowerBombOwner_l8567ec* owner) {
        reinterpret_cast<void (*)(UnkPowerBomb_l8567ec*, UnkPowerBombOwner_l8567ec*)>(vtable[0x290 / 8])(this, owner);
    }
};
static_assert(__builtin_offsetof(UnkPowerBomb_l8567ec, field_0x408) == 0x408, "layout");

struct UnkPowerBombGun_l8567ec {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual bool vf_0x48();

    // vtable slots 0x180 to 0x1a8: the bomb's settings.
    f32 vf_0x180() { return reinterpret_cast<f32 (*)(UnkPowerBombGun_l8567ec*)>(Vtable()[0x180 / 8])(this); }
    u32 vf_0x198() { return reinterpret_cast<u32 (*)(UnkPowerBombGun_l8567ec*)>(Vtable()[0x198 / 8])(this); }
    f32 vf_0x1a0() { return reinterpret_cast<f32 (*)(UnkPowerBombGun_l8567ec*)>(Vtable()[0x1a0 / 8])(this); }
    f32 vf_0x1a8() { return reinterpret_cast<f32 (*)(UnkPowerBombGun_l8567ec*)>(Vtable()[0x1a8 / 8])(this); }
    void** Vtable() { return *reinterpret_cast<void***>(this); }

    u8 pad_0x8[0x10];
    UnkPowerBombOwner_l8567ec* owner;  // 0x18
    u8 pad_0x20[0x70 - 0x20];
    CStrId script_name;  // 0x70
    u8 pad_0x78[0x1880 - 0x78];
    UnkQualifiedId ammo;  // 0x1880
    u8 pad_0x1890[0x18a0 - 0x1890];
    UnkWeakHandle* bomb;  // 0x18a0
};
static_assert(__builtin_offsetof(UnkPowerBombGun_l8567ec, bomb) == 0x18a0, "layout");

extern const u64 g_unk_710164ad38;  // the bomb's timeline key, reached through the GOT

UnkPowerBomb_l8567ec* fn_7100856b98(UnkPowerBombActor_l8567ec* actor);
void fn_7100ac7db4(UnkPowerBomb_l8567ec* bomb, const char* name, bool flag);

// Spawns the actor definition `id` through the factory.
static inline UnkPowerBombActor_l8567ec* Spawn(UnkActorDefFactory* factory, CStrId* id) {
    u32 type_id = g_unk_7172a0ba48;
    UnkPowerBombName_l8567ec* name = reinterpret_cast<UnkPowerBombName_l8567ec*>(id->data);
    ++name->use_count;
    const char* text = reinterpret_cast<UnkPowerBombName_l8567ec*>(id->data)->text;
    UnkPowerBombDefType_l8567ec type;
    type.id = type_id;
    return reinterpret_cast<UnkPowerBombActor_l8567ec*>(
        fn_710119438c(factory, text, id, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
}

bool fn_71008567ec(UnkPowerBombGun_l8567ec* self) {
    if (!self->vf_0x48())
        return false;

    UnkAnimationComponent* animation = self->owner ? self->owner->animation : nullptr;
    UnkTrackQuery query;
    query.cls = fn_7100d32fd8();
    query.result = nullptr;
    query.value = -1.0f;
    if (animation)
        fn_710117b08c(animation, &query);
    UnkActorDefFactory* factory = g_unk_71726a0220;
    UnkPowerBombActor_l8567ec* actor;
    if (query.result)
        actor = Spawn(factory, &query.result->sources);
    else
        actor = Spawn(factory, &GameManagerIdTable().field_0x1c68);
    if (!actor)
        return false;
    if (!fn_710009d50c(actor->vf_0x0(), fn_7100d70c20()))
        return false;

    if (self->owner && self->owner->inventory) {
        f32 count = fn_7101142928(self->owner->inventory, self->ammo);
        fn_7101142a28(self->owner->inventory, self->ammo, true, true, true, count + -1.0f, 0.0f);
    }

    if (UnkPowerBomb_l8567ec* bomb = fn_7100856b98(actor)) {
        CVector3D pos;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        CVector3D rot;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        bomb->Placement(self->owner, &pos, &rot);
        actor->SetPosition(&pos, true);
        actor->SetRotation(&rot, true);
        fn_7100d7c828(reinterpret_cast<UnkActor*>(actor), &g_unk_710164ad38, false);
        if (fn_7100dec788(g_unk_7101d47310)) {
            UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
            u64 hash = fn_7100001570("powerbombcharged", 16);
            fn_7100d7c94c(reinterpret_cast<UnkActor*>(player), &hash);
        }
        fn_7100ac7db4(bomb, "PowerBomb", false);
        bomb->vf_0x290(self->owner);
        fn_71008c1538(reinterpret_cast<UnkRequest*>(bomb->charge), self->vf_0x180());
        bomb->field_0x3fc = self->vf_0x198();
        bomb->field_0x408 = self->vf_0x1a0();
        bomb->field_0x40c = self->vf_0x1a8();
    }

    if (self->owner && self->owner->script) {
        ++self->script_name.data->reads;
        fn_7100305088(self->owner->script, self->script_name.data->text, g_unk_710162e2ed);
    }

    UnkWeakHandle* handle = fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(actor));
    if (self->bomb != handle) {
        if (handle)
            ++handle->ref_count;
        if (UnkWeakHandle* old = self->bomb) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        self->bomb = handle;
    }
    return true;
}
