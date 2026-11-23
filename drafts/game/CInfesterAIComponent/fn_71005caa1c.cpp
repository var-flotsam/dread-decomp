// CInfesterAIComponent's unit (tools/tu_map.py, unit 892): fn_71005caa1c enters the AI's state: after the base
// (fn_7100369650) it hands the actor to one of the manager's two sets, resets the state at +0x146c8 from its
// parameters, plays the actor's animations and stores the target's position as "vPlayerPos" in the blackboard.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTUProperty.h"
#include "game/behaviortree/CBehavior.h"
#include "game/UnkTypedValue.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

struct UnkNodeSet_b43a1;
struct UnkNodeSet_b44a2;
struct UnkAnimComponent_b52;
struct UnkActorTarget;

void fn_710057a224(UnkNodeSet_b43a1* self, u64 v);
void fn_71005cd094(UnkNodeSet_b44a2* self, u64 v);
void fn_7101178e08(UnkAnimComponent_b52* self, u32 value, bool apply, bool flag, f32 time);
void fn_710117662c(UnkActorTarget* self, u32 value, u32 extra, bool apply, bool flag, f32 time);

// The key of the name that keeps the state's flag at +0x18 clear.
static const u64 kNoFlagKey = 0x3a883c06e570d93eull;

struct UnkInfesterParams_l5caa1c {
    u8 pad_0x0[0x50c];
    u32 field_0x50c;
    CStrId name;  // 0x510
    u32 mode;     // 0x518
};

struct UnkInfesterFlags_l5caa1c {
    bool field_0x0;
    bool field_0x1;
};

struct UnkInfesterEnter_l5caa1c {
    UnkInfesterParams_l5caa1c* params;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    UnkInfesterFlags_l5caa1c flags;  // 0x14
    bool field_0x16;
    bool field_0x17;
    bool field_0x18;
    bool field_0x19;
    bool field_0x1a;
    u8 pad_0x1b[0x28 - 0x1b];
    void* field_0x28;
};

struct UnkInfesterEnterAI_l5caa1c {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    u8 pad_0x38[0x1260 - 0x38];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1350[0x146c8 - 0x1260 - sizeof(CBlackboard)];
    UnkInfesterEnter_l5caa1c enter;  // 0x146c8
};

static inline UnkManager_71726af468* Manager() {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    return manager;
}

static inline CVector3D UnkTranslation(const UnkActor* actor) {
    CVector3D v;
    v.x = actor->field_0x44;
    v.y = actor->field_0x54;
    v.z = actor->field_0x64;
    return v;
}

void fn_71005caa1c(UnkInfesterEnterAI_l5caa1c* self) {
    UnkInfesterEnter_l5caa1c* enter = &self->enter;
    fn_7100369650(reinterpret_cast<char*>(self));
    u32 mode = enter->params->mode;
    UnkManager_71726af468* manager = Manager();
    // The manager's two sets, at +0x4f0 and +0x568.
    if (mode)
        fn_710057a224(reinterpret_cast<UnkNodeSet_b43a1*>(&manager->field_0x330[0x4f0 - 0x330]),
                      reinterpret_cast<u64>(self->actor));
    else
        fn_71005cd094(reinterpret_cast<UnkNodeSet_b44a2*>(&manager->field_0x330[0x568 - 0x330]),
                      reinterpret_cast<u64>(self->actor));

    UnkInfesterParams_l5caa1c* params = enter->params;
    enter->field_0x8 = 0;
    enter->field_0xc = params->field_0x50c;
    enter->field_0x10 = 0;
    enter->flags = UnkInfesterFlags_l5caa1c{false, true};
    enter->field_0x16 = false;
    enter->field_0x17 = false;
    enter->field_0x18 = true;
    enter->field_0x19 = false;
    enter->field_0x1a = false;
    enter->field_0x28 = nullptr;
    if (CStrId(params->name).data->Key() == kNoFlagKey)
        enter->field_0x18 = false;

    if (void* anim = self->actor->field_0x220) {
        fn_7101178e08(static_cast<UnkAnimComponent_b52*>(anim), 0xb, false, false, -1.0f);
        fn_7101178e08(static_cast<UnkAnimComponent_b52*>(anim), 0xc, false, false, -1.0f);
        fn_710117662c(static_cast<UnkActorTarget*>(anim), 6, 0, false, false, -1.0f);
        if (void* model = self->actor->field_0x228) {
            CStrId name(enter->params->name);
            u64 key = name.data->Key();
            fn_7101172764(static_cast<UnkFlagTarget*>(model), reinterpret_cast<const char*>(&key));
        }
    }

    CBlackboard* blackboard = &self->blackboard;
    UnkTypedValue v;
    CStrId name("vPlayerPos", true);
    CVector3D pos = UnkTranslation(fn_7100d7c990(self->actor));
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            v.type = fn_71000246e4();
            v.value = &pos;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&name), &v);
        }
    }
}
