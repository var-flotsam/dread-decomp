// CTakumakuAIComponent's unit (tools/tu_map.py), fn_71006d56d0: sets id 0x200 on the owner's (+0x30)
// controller (+0x220) when the owner stands high enough above its target (the test of fn_71006d57f8 in
// CTakumakuAIComponent_Above_b66a4.cpp, inlined here) and flag 9 is clear, and clears it when the owner is
// below and flag 9 is set. Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/CTakumakuAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2689.h"
#include "types.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp declares them.
struct UnkShakernautCtrl_b53;
struct UnkShakernautId_b53 {
    u32 value;
    u32 unk_0x4;
};
// As src/game/CAnimationComponent/CAnimationComponent_Flags_b47a3.cpp defines it.
struct UnkAnimFlagsHolder_b47a3;
// As src/game/CPlayerMovement/CPlayerMovement_CachedComponent.cpp defines it.
struct UnkPlayerMovementCache;

namespace {
struct UnkTakumakuBounds_A3_b76 {
    virtual void vf_0x0();
    u8 pad_0x8[0x44 - 0x8];
    f32 min_y;  // 0x44
    u8 pad_0x48[4];
    f32 max_y;  // 0x4c
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60
    void Refresh() { reinterpret_cast<void (*)(UnkTakumakuBounds_A3_b76*)>((*reinterpret_cast<void***>(this))[0x98 / 8])(this); }
};
}  // namespace

struct UnkTakumakuAI_A3_b76 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

// fn_71006d57f8's body.
static inline __attribute__((always_inline)) bool IsAbove(UnkTakumakuAI_A3_b76* self) {
    UnkActor* owner = self->owner;
    if (!owner)
        return false;
    void* cls = owner->field_0x220;
    UnkActor* target = fn_7100d7c990(owner);
    if (!cls || !target)
        return false;
    f32 y = target->field_0x54;
    UnkPlayerMovementCache* cache = *reinterpret_cast<UnkPlayerMovementCache**>(reinterpret_cast<char*>(target) + 0x200);
    if (cache) {
        if (auto* b = static_cast<UnkTakumakuBounds_A3_b76*>(fn_71010d7aac(cache))) {
            if (b->dirty)
                b->Refresh();
            y = (b->min_y + b->max_y) * 0.5f;
        }
    }
    return self->owner->field_0x54 - y >= g_unk_7101a67de8;
}

void fn_71006d56d0(UnkTakumakuAI_A3_b76* self) {
    UnkActor* owner = self->owner;
    if (!owner)
        return;
    UnkShakernautCtrl_b53* ctrl = static_cast<UnkShakernautCtrl_b53*>(owner->field_0x220);
    if (!ctrl)
        return;
    bool above = IsAbove(self);
    goto compare;
    UnkShakernautId_b53 a;
    UnkShakernautId_b53 b;
compare:
    if (!fn_7101178524(reinterpret_cast<UnkAnimFlagsHolder_b47a3*>(ctrl), 9) && above) {
        a.value = 0x200;
        fn_7101178564(ctrl, &a, true, true);
    } else if (fn_7101178524(reinterpret_cast<UnkAnimFlagsHolder_b47a3*>(ctrl), 9) && !above) {
        b.value = 0x200;
        fn_71011785a8(ctrl, &b, true, true);
    }
}
