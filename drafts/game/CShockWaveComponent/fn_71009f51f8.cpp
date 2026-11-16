// CShockWaveComponent's unit (tools/tu_map.py unit 1505): spawns the shock wave's actor by its name (+0x1b0)
// from the actor-definition factory, adds it to the spawning actor's spawn group, hands it this object
// (virtual 0x168 of its "SHOCKWAVE" component) and starts the effect at +0x150 where it stands.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CDrop.h"
#include "game/CLifeComponent.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2527.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU344.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::reflection::CClass;

struct UnkSpawnGroupList_ovb0065;
struct UnkWeakTarget;
void fn_710111b96c(UnkSpawnGroupList_ovb0065* self, UnkWeakTarget* target);
void* fn_710048fba4(void* owner);  // refl_FindTyped of "SHOCKWAVE".

struct alignas(8) UnkPowerUpLifeType_b52 {
    u32 id;
};

// The spawned actor: its class (virtual 0) and its transform.
struct UnkShockWaveActor_l9f51f8 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38

    const CClass* GetClass() { return reinterpret_cast<const CClass* (*)(UnkShockWaveActor_l9f51f8*)>(vtable[0])(this); }
};
static_assert(__builtin_offsetof(UnkShockWaveActor_l9f51f8, xform.flags) == 0x90, "layout");

// The spawning actor's link to its spawn group, resolved on demand (fn_710115fe20).
struct UnkGroupLink_l9f51f8 {
    u8 pad_0x0[0x10];
    char* object;  // 0x10
};

struct UnkGroupHolder_l9f51f8 {
    u8 pad_0x0[0x48];
    UnkGroupLink_l9f51f8* link;  // 0x48

    char* Linked() { return link ? link->object : nullptr; }
};

struct UnkGroupTarget_l9f51f8 {
    u8 pad_0x0[0x10];
    void* group;  // 0x10
};

struct UnkSpawner_l9f51f8 {
    u8 pad_0x0[0x1b8];
    UnkGroupHolder_l9f51f8* holder;  // 0x1b8
};

struct UnkShockWave_l9f51f8 {
    u8 pad_0x0[0x150];
    CFilePathStrId effect;  // 0x150
    u8 pad_0x158[0x58];
    CStrId name;  // 0x1b0
};

// The spawned actor's shock wave component.
struct UnkShockWaveComponent_l9f51f8 {
    void** vtable;

    void SetSource(UnkShockWave_l9f51f8* source) {
        reinterpret_cast<void (*)(UnkShockWaveComponent_l9f51f8*, UnkShockWave_l9f51f8*)>(vtable[0x168 / 8])(this,
                                                                                                         source);
    }
};

struct UnkFxVtable_l9f51f8 {
    void* unk_0x0[2];
    void (*set_position)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkFxObject_ovb0041* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkFxObject_ovb0041* self, bool loop);   // 0x90
    void (*set_speed)(UnkFxObject_ovb0041* self, f32 speed);  // 0x98
};

struct UnkFx_l9f51f8 {
    UnkFxVtable_l9f51f8* vtable;
    u8 pad_0x8[0x100];
    f32 field_0x108;
    u8 pad_0x10c[0x4e];
    bool field_0x15a;
};
static_assert(__builtin_offsetof(UnkFx_l9f51f8, field_0x15a) == 0x15a, "layout");

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

UnkShockWaveActor_l9f51f8* fn_71009f51f8(UnkShockWave_l9f51f8* self, UnkSpawner_l9f51f8* spawner) {
    UnkActorDefFactory* factory = g_unk_71726a0220;
    u32 type = g_unk_7172a0ba48;
    ++self->name.data->reads;
    auto* actor = reinterpret_cast<UnkShockWaveActor_l9f51f8*>(
        fn_710119438c(factory, reinterpret_cast<const char*>(&self->name.data->String()), &self->name,
                      UnkPowerUpLifeType_b52{type}, nullptr));
    if (!actor)
        return nullptr;
    if (!fn_710009d50c(actor->GetClass(), fn_7100d70c20()))
        return nullptr;

    if (spawner) {
        if (UnkGroupHolder_l9f51f8* holder = spawner->holder) {
            char* object = holder->Linked();
            if (!object) {
                fn_710115fe20(reinterpret_cast<UnkHolder*>(holder));
                object = holder->Linked();
            }
            if (object) {
                auto* target = reinterpret_cast<UnkGroupTarget_l9f51f8*>(
                    *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(object + 0x60)));
                if (target && target->group) {
                    if (void* group = fn_71007c04f0(target->group))
                        fn_710111b96c(static_cast<UnkSpawnGroupList_ovb0065*>(group),
                                      reinterpret_cast<UnkWeakTarget*>(actor));
                }
            }
        }
    }

    if (auto* component = static_cast<UnkShockWaveComponent_l9f51f8*>(fn_710048fba4(actor)))
        component->SetSource(self);

    if (self->effect.data == &g_unk_7101d47200->empty)
        return actor;

    UnkModelManager* manager = ModelManager();
    UnkVec3f position = actor->xform.GetTranslation();
    UnkVec3f rotation = actor->xform.GetAngle();
    UnkVec3f scale(1.0f, 1.0f, 1.0f);
    UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, &self->effect, false, true);
    if (fx) {
        UnkFx_l9f51f8* view = reinterpret_cast<UnkFx_l9f51f8*>(fx);
        view->vtable->set_position(fx, &position);
        view->vtable->set_rotation(fx, &rotation);
        view->vtable->set_scale(fx, &scale);
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx), true);
        view->vtable->set_loop(fx, false);
        view->vtable->set_speed(fx, 1.0f);
        fn_71011a9bb4(manager, fx);
        view->field_0x15a = false;
        view->field_0x108 = 1.0f;
    }
    return actor;
}
