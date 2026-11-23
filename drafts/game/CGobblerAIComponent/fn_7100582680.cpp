// CGobblerAIComponent (tools/tu_map.py): after the base AI handler, sends the owner's animation (+0x220 of the
// owner at +0x30) a mode-2 request whose descriptor depends on how far apart along x the owner and the other
// actor are (600 or more, 500 to 600, 400 to 500, closer). Names and layouts are placeholders.
#include <cmath>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkHashedString;

extern char g_unk_71726e5158[] __attribute__((visibility("hidden")));
extern char g_unk_71726e5168[] __attribute__((visibility("hidden")));
extern char g_unk_71726e5178[] __attribute__((visibility("hidden")));
extern char g_unk_71726e5188[] __attribute__((visibility("hidden")));

struct UnkGobblerAI_l582680 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

// The id table's slot at +0x8c0 from its +0xb18 padding: UnkGameManagerIdTable has no member for +0xc80 yet.
inline u64 RequestHash_l582680() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    const CStrId& id = reinterpret_cast<const CStrId*>(&s_table)[0xc80 / sizeof(CStrId)];
    return static_cast<const UnkHashedString*>(id.data)->Key();
}

static inline void Send_l582680(void* animation, const void* desc, base::reflection::UnkReflTypeIdArg* mode,
                                base::reflection::UnkReflTypeIdArg* mask) {
    u64 hash = RequestHash_l582680();
    mode->id = 2;
    mask->id = UnkAll_SpitSend().value;
    fn_7101176690(animation, hash, 0xf4061ff25a2163d6ull, mode, &g_unk_710164b958, desc, mask);
}

void fn_7100582680(UnkGobblerAI_l582680* self, UnkActor* other) {
    fn_7100369d80(reinterpret_cast<UnkWarriorSend_b58*>(self), other);
    void* animation = self->owner->field_0x220;
    if (!animation)
        return;
    f32 distance = fabsf(self->owner->field_0x44 - other->field_0x44);
    if (distance >= 600.0f)
        goto far;
    if (distance < 600.0f && distance >= 500.0f)
        goto mid;
    if ((distance < 500.0f) & (distance >= 400.0f))
        goto near;
    goto close;
    // The gotos skip these declarations, so they get no lifetime markers and keep slots of their own.
    base::reflection::UnkReflTypeIdArg far_mode, far_mask;
    base::reflection::UnkReflTypeIdArg mid_mode, mid_mask;
    base::reflection::UnkReflTypeIdArg near_mode, near_mask;
    base::reflection::UnkReflTypeIdArg close_mode, close_mask;
far:
    Send_l582680(animation, g_unk_71726e5158, &far_mode, &far_mask);
    return;
mid:
    Send_l582680(animation, g_unk_71726e5168, &mid_mode, &mid_mask);
    return;
near:
    Send_l582680(animation, g_unk_71726e5178, &near_mode, &near_mask);
    return;
close:
    Send_l582680(animation, g_unk_71726e5188, &close_mode, &close_mask);
}
