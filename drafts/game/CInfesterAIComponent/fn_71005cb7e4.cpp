// CInfesterAIComponent's unit (tools/tu_map.py, unit 892): fn_71005cb7e4 spawns the AI's projectile, a
// "fulmitebellymine" or an "infesterball", places it at the owner's spawn point (or the owner's position) and
// hands the owner to the projectile's AI component. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CScenario.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkInfesterAI_b57a3;
void fn_71005cb6b4(UnkInfesterAI_b57a3* self, bool on);

struct alignas(8) UnkPowerUpLifeType_b52 {
    u32 id;
};

extern u32 g_unk_7172a0ba4c;  // the actor definition type the factory is asked for
// The name of the owner's node the projectile starts at.
extern const char g_unk_710164a110[] __attribute__((visibility("hidden")));

void fn_710057c3c8(UnkReflObject* ai, UnkActor* owner);  // CFulmiteBellyMineAIComponent
void fn_71005cf028(UnkReflObject* ai, UnkActor* owner);  // CInfesterBallAIComponent

// The projectile's AI component.
struct UnkSpawnedAI_l5cb7e4 {
    void** vtable;

    void vf_0x508() { reinterpret_cast<void (*)(UnkSpawnedAI_l5cb7e4*)>(vtable[0x508 / 8])(this); }
};

struct UnkInfesterSpit_l5cb7e4 {
    u32 field_0x0;
    u8 pad_0x4;
    bool field_0x5;
    u8 pad_0x6[3];
    bool field_0x9;
    u8 pad_0xa[0x18 - 0xa];
    UnkActor* projectile;  // 0x18
};

struct UnkInfesterSpitAI_l5cb7e4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146d8 - 0x38];
    UnkInfesterSpit_l5cb7e4 spit;  // 0x146d8
};

// Whether a spawned object is an entity.
static inline bool IsEntity(UnkReflObject* spawned) {
    return fn_710009d50c(spawned->vf_0x0(), fn_7100d70c20());
}

static inline CVector3D UnkTranslation(const UnkActor* actor) {
    CVector3D v;
    v.x = actor->field_0x44;
    v.y = actor->field_0x54;
    v.z = actor->field_0x64;
    return v;
}

void fn_71005cb7e4(UnkInfesterSpitAI_l5cb7e4* self, bool mine) {
    UnkInfesterSpit_l5cb7e4* spit = &self->spit;
    spit->field_0x0 = 0;
    fn_71005cb6b4(reinterpret_cast<UnkInfesterAI_b57a3*>(self), true);
    spit->field_0x5 = false;
    // The factory is asked for the actor by name; the result counts only when it is an entity.
    UnkReflObject* spawned;
    if (mine) {
        CStrId name("fulmitebellymine", true);
        UnkActorDefFactory* factory = g_unk_71726a0220;
        ++name.data->reads;
        spawned = reinterpret_cast<UnkReflObject*>(
            fn_710119438c(factory, reinterpret_cast<const char*>(&name.data->String()), &name,
                          UnkPowerUpLifeType_b52{g_unk_7172a0ba4c}, nullptr));
        spawned = spawned && IsEntity(spawned) ? spawned : nullptr;
    } else {
        CStrId name("infesterball", true);
        UnkActorDefFactory* factory = g_unk_71726a0220;
        ++name.data->reads;
        spawned = reinterpret_cast<UnkReflObject*>(
            fn_710119438c(factory, reinterpret_cast<const char*>(&name.data->String()), &name,
                          UnkPowerUpLifeType_b52{g_unk_7172a0ba4c}, nullptr));
        spawned = spawned && IsEntity(spawned) ? spawned : nullptr;
    }
    if (!spawned)
        return;
    UnkActor* projectile = reinterpret_cast<UnkActor*>(spawned);
    spit->projectile = projectile;
    spit->field_0x9 = true;

    CVector3D pos = UnkTranslation(self->owner);
    UnkMatrix44 m;
    for (u32 i = 0; i < 16; ++i)
        m.m[i] = 0.0f;
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    if (fn_71011735e8(static_cast<UnkEntity*>(self->owner->field_0x228), g_unk_710164a110, &m, 0)) {
        pos.x = m.m[3];
        pos.y = m.m[7];
        pos.z = m.m[11];
    }
    projectile->vf_0xd0(reinterpret_cast<const UnkVec3f*>(&pos), true);
    fn_71011a2770(g_unk_7101d47310->field_0x608, projectile);

    UnkReflObject* ai = static_cast<UnkReflObject*>(projectile->field_0x240);
    if (mine)
        fn_710057c3c8(ai && fn_710009d50c(ai->vf_0x0(), fn_710057c144()) ? ai : nullptr, self->owner);
    else
        fn_71005cf028(ai && fn_710009d50c(ai->vf_0x0(), fn_71005ceda4()) ? ai : nullptr, self->owner);
    if (ai)
        reinterpret_cast<UnkSpawnedAI_l5cb7e4*>(ai)->vf_0x508();
}
