// CCooldownXBossAIComponent (tools/tu_map.py): switches the entity named by the lazily resolved handle
// at +0x15748 (source +0x15718, state +0x15740; the shape of CTutoExitLogicAction_b47.cpp) on or off
// (bit 0 of `on`, fn_7100232864). Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/UnkTU341.h"
#include "base/UnkDestructors.h"
#include "game/UnkTU437.h"
#include "types.h"

struct UnkEntityState;

struct UnkCountedCooldown_b49 : base::UnkRefCounted {
    void* field_0x10;
};

using UnkRef = base::UnkRefPtrMember;

struct UnkCooldownAI_b49 {
    u8 pad_0x0[0x15718];
    u8 source[0x28];
    u32 state;
    u32 pad_0x15744;
    base::UnkRefHandle handle;
};

// Resolves the handle on first use and returns the entity it names, or null.
static inline UnkEntityState* GetHandleEntity(UnkCooldownAI_b49* self) {
    if (self->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    auto* c = static_cast<UnkCountedCooldown_b49*>(self->handle.ptr);
    return c ? static_cast<UnkEntityState*>(c->field_0x10) : nullptr;
}

void fn_71004d432c(UnkCooldownAI_b49* self, u32 on) {
    if (GetHandleEntity(self))
        fn_7100232864(GetHandleEntity(self), on & 1);
}
