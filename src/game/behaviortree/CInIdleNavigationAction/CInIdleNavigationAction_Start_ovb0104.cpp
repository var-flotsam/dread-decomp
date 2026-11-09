// behaviortree::CInIdleNavigationAction's unit (unit 487): the action's start: after the base start,
// caches the owner's navigation (+0x298); unless the owner's AI is in state 4 it sends the navigation
// a kind-5 parameter block; then records whether the owner's animation has a turn track. Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkStruct_0x68;
void fn_7100394bd0(UnkStruct_0x68* self);
u32 fn_71010cc3a4(void*);

struct UnkIdleNavigationOwner {
    u8 pad_0x0[0x218];
    void* ai;  // 0x218
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x70];
    void* navigation;  // 0x298
};

struct UnkInIdleNavigationAction {
    u8 pad_0x0[0x18];
    UnkIdleNavigationOwner* owner;  // 0x18
    u8 pad_0x20[0x10];
    void* navigation;  // 0x30
    bool ai_ready;  // 0x38
    bool turning;  // 0x39
};

void fn_710038a590(UnkInIdleNavigationAction* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkIdleNavigationOwner* owner = self->owner;
    self->navigation = owner->navigation;
    if (!self->navigation)
        return;
    void* ai = owner->ai;
    if (ai && fn_71010cc3a4(ai) == 4) {
        self->ai_ready = true;
    } else {
        self->ai_ready = false;
        UnkParams params;
        params.field_0x1c = 0;
        __builtin_memset(params.field_0x8, 0, 13);
        __builtin_memset(params.field_0x20, 0, 16);
        params.field_0x30 = 0;
        params.field_0x34 = 5;
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        *reinterpret_cast<u16*>(&params.pad_0x38) = 0x101;
        fn_71006f4b0c(self->navigation, &params, 1);
    }
    UnkAnimationComponent* animation = self->owner->animation;
    if (animation && fn_710117b348(animation, fn_7100d2b3a8()))
        self->turning = true;
    else
        self->turning = false;
}
