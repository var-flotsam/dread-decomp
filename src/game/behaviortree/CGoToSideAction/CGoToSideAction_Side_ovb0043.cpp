// behaviortree::CGoToSideAction's unit (tools/tu_map.py): fn_7100380e48 picks the side and sends the
// navigation there. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/behaviortree/CGoToSideAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

// The kind-6 parameter block (as src/game/CCaterzillaAIComponent/fn_71003f832c.cpp), with a callback.
struct UnkGoToSideParams_ovb0043 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[3];
    f32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    void* object;  // 0x20
    void (*fn)();  // 0x28
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 field_0x38;
    u8 field_0x39;
};

struct UnkGoToSideActor_ovb0043 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x70];
    void* navigation;  // 0x298
};

struct UnkGoToSideAction_ovb0043 {
    u8 pad_0x0[0x18];
    UnkGoToSideActor_ovb0043* actor;  // 0x18
    u8 pad_0x20[0x10];
    u8 field_0x30[8];
    bool side;  // 0x38
    bool found;  // 0x39
};

bool fn_7100394bdc(UnkGoToSideAction_ovb0043* self, void* out, bool* side, const base::global::CStrId& id, bool flag);
void fn_7100381224();

// Picks the side (fn_7100394bdc, kept at +0x39); unless the actor is already in a CCharClassTurnTrack
// track, asks its navigation (+0x298) to go there, called back through fn_7100381224.
bool fn_7100380e48(UnkGoToSideAction_ovb0043* self) {
    void* navigation = self->actor->navigation;
    bool found = fn_7100394bdc(self, self->field_0x30, &self->side, g_unk_7101d470e0, false);
    self->found = found;
    if (!found || !navigation)
        return false;
    if (UnkGoToSideActor_ovb0043* actor = self->actor) {
        if (UnkAnimationComponent* animation = actor->animation) {
            if (fn_710117b348(animation, fn_7100d2b3a8()))
                return true;
        }
    }
    UnkGoToSideParams_ovb0043 params;
    __builtin_memset(params.field_0x8, 0, 13);
    params.field_0x30 = 0;
    params.object = nullptr;
    params.fn = nullptr;
    params.vtable = g_unk_7101a25fb0 + 2;
    params.field_0x34 = 6;
    params.field_0x38 = self->side;
    params.field_0x39 = 1;
    params.field_0x18 = 1.0f;
    params.field_0x1c = 1;
    params.object = self;
    params.fn = fn_7100381224;
    params.field_0x30 = 1;
    fn_71006f4b0c(navigation, &params, 1);
    return true;
}
