// behaviortree::CAgitateInNervousnessAction (tools/tu_map.py unit 995): starts the animation. When the
// owner has a character class (+0x220): if its AI component has a navigation object, that object gets a
// kind-5 parameter block (game_s2/UnkParams.h); then the class is sent the animation id at +0x38 with
// mode 0 (as src/game_s2/UnkSpitSend.h). Names and layouts are placeholders (the action's layout is
// CAgitateInNervousnessAction.cpp's).
#include "../../../game_s2/UnkSpitSend.h"
#include "game/UnkAIComponent.h"
#include "game/UnkActor.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkAgitateAnim_ain {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkAgitateAction_ain {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0xc];
    bool active;  // 0x2c
    f32 limit;  // 0x30
    f32 time;  // 0x34
    UnkAgitateAnim_ain* anim;  // 0x38
};

void fn_71006749bc(UnkAgitateAction_ain* self) {
    UnkActor* owner = self->owner;
    void* cls = owner->field_0x220;
    if (!cls)
        return;
    // Declared past a goto (no lifetime markers), so each keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg mode;
    UnkParams params;
    base::reflection::UnkReflTypeIdArg mask;
start:
    if (owner && owner->field_0x240) {
        if (UnkNavigation* nav = fn_71010fccf8(static_cast<UnkAIComponent*>(owner->field_0x240))) {
            __builtin_memset(params.field_0x8, 0, 13);
            params.field_0x30 = 0;
            __builtin_memset(params.field_0x20, 0, 16);
            params.vtable = g_unk_7101a25b10 + 2;
            params.field_0x39 = 0;
            params.field_0x3b = 0;
            params.field_0x3d = 0;
            params.field_0x34 = 5;
            params.field_0x18 = 0;
            params.field_0x1c = 1;
            fn_71006f4b0c(nav, &params, 0);
        }
    }
    u64 id = self->anim->id;
    mode.id = 0;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(cls, id, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}
