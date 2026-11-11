// CTakumakuAIComponent's unit (tools/tu_map.py): whether the owner (+0x30) stands at least the tuned
// height (the f32 at g_unk_7101a67de8) above its target (fn_7100d7c990). The target's height is its
// y, or the middle of the bounds its +0x200's cached component (fn_71010d7aac) keeps, refreshed
// (vf_0x98) when flagged. False without the owner, its +0x220 or a target. Names and layouts are
// placeholders.
#include "game/CTakumakuAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2689.h"
#include "types.h"

// As src/game/CPlayerMovement/CPlayerMovement_CachedComponent.cpp defines it.
struct UnkPlayerMovementCache;

struct UnkTakumakuBounds_b66a4 {
    virtual void vf_0x0();
    u8 pad_0x8[0x44 - 0x8];
    f32 min_y;  // 0x44
    u8 pad_0x48[4];
    f32 max_y;  // 0x4c
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60
    void Refresh() { reinterpret_cast<void (*)(UnkTakumakuBounds_b66a4*)>((*reinterpret_cast<void***>(this))[0x98 / 8])(this); }
};

struct UnkTakumakuAI_b66a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

bool fn_71006d57f8(UnkTakumakuAI_b66a4* self) {
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
        if (auto* b = static_cast<UnkTakumakuBounds_b66a4*>(fn_71010d7aac(cache))) {
            if (b->dirty)
                b->Refresh();
            y = (b->min_y + b->max_y) * 0.5f;
        }
    }
    return self->owner->field_0x54 - y >= g_unk_7101a67de8;
}
