// CShootActivatorComponent (unit of CCharClassShootActivatorComponent, tools/tu_map.py): after the base step,
// reads the activator's flag (g_unk_717272c420) from the given blackboard's section of the current scenario
// and, when it is set, restarts the timer (+0x4bc) from the duration (+0x4b0), activates the entities under
// the empty id (fn_71008910a0), resolves the handle at +0x568 once (state +0x560) and gives the object it
// names the key g_unk_710164aed0 (fn_7100d7c828). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CCharClassShootActivatorComponent.h"
#include "game/CItemLifeComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU437.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkActivatableComponent;
void fn_71008910a0(UnkActivatableComponent* component, CStrId* name, u32 mode);

extern u64 g_unk_710164aed0;

struct UnkShootActivatorValue_b48 {
    CStrId id;
    u64 field_0x8;
};

// The object the handle at +0x568 refers to: the actor it names at +0x10.
struct UnkShootActivatorTarget_l890ea4 : base::UnkRefCounted {
    UnkActor* actor;
};

struct UnkShootActivator_l890ea4 {
    u8 pad_0x0[0x488];
    UnkShootActivatorValue_b48* values;
    u64* keys;
    u32 count;
    u8 pad_0x49c[0x14];
    f32 duration;  // 0x4b0
    u8 pad_0x4b4[8];
    f32 timer;  // 0x4bc
    u8 pad_0x4c0[0x78];
    u8 source[0x28];  // 0x538
    u32 state;        // 0x560
    u32 pad_0x564;
    base::UnkRefHandle handle;  // 0x568
};

// Activates the entities listed under `name` (fn_71008910a0, mode 1).
static inline void Activate(UnkShootActivator_l890ea4* self, CStrId name) {
    fn_71008910a0(reinterpret_cast<UnkActivatableComponent*>(self), &name, 1);
}

void fn_7100890ea4(UnkShootActivator_l890ea4* self, CBlackboard* blackboard) {
    fn_71008e0adc(reinterpret_cast<UnkPowerUpLifeBase_ovb0126*>(self), blackboard);
    CBlackboard::CSection* section = fn_71010472e0(blackboard, fn_71011a2fbc(g_unk_7101d47310->field_0x608));
    if (!section)
        return;
    bool activated = false;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717272c420.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    bool found = fn_710006d11c(section, &self->values[index].id, &activated);
    if (!activated || !found)
        return;
    self->timer = self->duration;
    Activate(self, g_unk_7101d470e0);
    if (self->state == 1) {
        {
            base::UnkRefPtrMember fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    auto* target = static_cast<UnkShootActivatorTarget_l890ea4*>(self->handle.ptr);
    if (target && target->actor)
        fn_7100d7c828(target->actor, &g_unk_710164aed0, false);
}
