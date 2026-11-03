// behaviortree::CSwitchBetweenAIModesAction (tools/tu_map.py): the node's exit. After the base routine,
// on result 5 it plays the two animations once (+0x4c, then +0x48 with the flags at +0x78/+0x79, on the
// animation component at +0x88; marks +0x7a) and hands the pair at +0x48 to the AI (+0x80, slot
// 0x618); then it unregisters its listener (+0x98) from the watched object behind the weak handle at
// +0xb0 and drops the handle. Names and layouts are placeholders.
#include "game/UnkAnimationComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using UnkActorTarget = UnkAnimationComponent;
void fn_710117662c(UnkActorTarget* target, u32 a, u32 b, u32 c, u32 d, f32 value);
void fn_7101178e08(UnkActorTarget* target, u32 a, u32 b, u32 c, f32 value);

struct UnkAIModePair_b49 {
    u32 first;
    u32 second;
};

struct UnkAIModesAI_b49 {
    void** vtable;
    void vf_0x618(u32 a, u32 b) {
        reinterpret_cast<void (*)(UnkAIModesAI_b49*, u32, u32)>(vtable[0x618 / 8])(this, a, b);
    }
};

struct UnkSwitchAIModes_b49 {
    u8 pad_0x0[0x48];
    UnkAIModePair_b49 modes;
    u8 pad_0x50[0x28];
    bool field_0x78;
    bool field_0x79;
    bool played;
    u8 pad_0x7b[5];
    UnkAIModesAI_b49* ai;
    UnkActorTarget* anim;
    u8 pad_0x90[0x8];
    u8 link[0x18];
    UnkWeakHandle* handle;
};

void fn_710036bd8c(UnkSwitchAIModes_b49* self, u32 result) {
    fn_7100394bd4(self);
    if (result == 5) {
        if (!self->played) {
            if (self->anim) {
                if (self->modes.second)
                    fn_7101178e08(self->anim, self->modes.second, 0, 0, -1.0f);
                if (self->modes.first)
                    fn_710117662c(self->anim, self->modes.first, 0, self->field_0x78, self->field_0x79, -1.0f);
            }
            self->played = true;
        }
        if (self->ai)
            self->ai->vf_0x618(self->modes.first, self->modes.second);
    }
    if (UnkWeakHandle* handle = self->handle) {
        if (handle->target) {
            fn_710117b3f0(handle->target, self->link);
            if (!self->handle)
                return;
        }
        ResetRef(&self->handle);
    }
}
