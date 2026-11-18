// CShelmitAIComponent unit (tools/tu_map.py): the damage handler. The base handler's answer wins; a hit
// neither thawing the owner nor flagged is dropped (answer 2); in mode 1 a flagged hit the state at +0x146c8
// does not take releases the event's object and starts the plasma ray (answer 6).
// Names and layouts are placeholders.
#include "game/CFrozenComponent.h"
#include "game/CShelmitAIComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

bool fn_710110583c(UnkRobotSoldierAI_A2_b69* ai);
extern u32 g_unk_7101a63bb8;

struct UnkShelmitEventObject_l6bb110 {
    void** vtable;
    u32 refs;
    u32 pad_0xc;
    void* field_0x10;

    void Release() {
        if (--refs == 0)
            reinterpret_cast<void (*)(UnkShelmitEventObject_l6bb110*)>(vtable[0x10 / 8])(this);
    }
};

struct UnkShelmitEvent_l6bb110 {
    u8 pad_0x0[0x8];
    UnkShelmitEventObject_l6bb110* object;  // 0x8
    u8 pad_0x10[0xb4 - 0x10];
    u32 flags;  // 0xb4
};

struct UnkShelmitOwner_l6bb110 {
    u8 pad_0x0[0x2c8];
    UnkFrostbiteComponent* frozen;  // 0x2c8
};

struct UnkShelmitState_l6bb110 {
    void** vtable;

    u32 Take(UnkShelmitOwner_l6bb110* owner, void* object, UnkShelmitEvent_l6bb110* event) {
        return reinterpret_cast<u32 (*)(UnkShelmitState_l6bb110*, UnkShelmitOwner_l6bb110*, void*,
                                        UnkShelmitEvent_l6bb110*)>(vtable[0x70 / 8])(this, owner, object, event);
    }
};

struct UnkShelmitStateBlock_l6bb110 {
    UnkShelmitState_l6bb110* state;  // 0x0
    u8 pad_0x8[0xb0];
    u32 mode;  // 0xb8
};

struct UnkShelmitDamageAI_l6bb110 {
    u8 pad_0x0[0x30];
    UnkShelmitOwner_l6bb110* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    UnkShelmitStateBlock_l6bb110 block;  // 0x146c8
};
static_assert(__builtin_offsetof(UnkShelmitDamageAI_l6bb110, block) == 0x146c8, "");

u32 fn_71006bb110(UnkShelmitDamageAI_l6bb110* self, UnkShelmitEvent_l6bb110* event) {
    UnkShelmitStateBlock_l6bb110* block = &self->block;
    u32 result = fn_7101105e44(reinterpret_cast<UnkSluggerAI*>(self), reinterpret_cast<UnkSluggerEvent*>(event));
    UnkFrostbiteComponent* frozen = self->owner->frozen;
    bool thawing = frozen && fn_71007f9f20(frozen) && !fn_71007f9fd0(frozen);
    u32 flags = event->flags;
    u32 mode = block->mode;
    bool flagged = mode != 0 || (g_unk_7101a63bb8 & flags) != 0;
    if (result)
        return result;
    if (!thawing && !flagged)
        return 2;
    if (mode != 1 || !(flags & 0x40800))
        return 0;
    UnkRobotSoldierAI_A2_b69* ai = reinterpret_cast<UnkRobotSoldierAI_A2_b69*>(self);
    bool ready = !fn_71011057fc(ai) && !fn_710110583c(ai);
    if (ready) {
        UnkShelmitState_l6bb110* state = block->state;
        if (state) {
            UnkShelmitOwner_l6bb110* owner = self->owner;
            UnkShelmitEventObject_l6bb110* object = event->object;
            u32 taken = state->Take(owner, object ? object->field_0x10 : nullptr, event);
            ready = !(taken == 1 || taken == 2) && ready;
        }
        if (ready) {
            if (event->object) {
                event->object->Release();
                event->object = nullptr;
            }
            fn_71006baf90(reinterpret_cast<UnkShelmitAI*>(self));
            return 6;
        }
    }
    return 0;
}
