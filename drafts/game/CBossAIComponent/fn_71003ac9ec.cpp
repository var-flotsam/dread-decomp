// CBossAIComponent's unit (tools/tu_map.py unit 549): after the base step (fn_7100369508), when the
// flags object allows it, resets the colour-slot timer (+0x1480c) and, for each of the 15 slot keys the
// linked target's slot table knows, sets that slot to white (kind 2), records it (index, id, colour) at
// +0x14818 and requests kind 1 of it with the cleared value. Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkSlots.h"
#include "game/UnkTU218.h"
#include "game/UnkTU253.h"
#include "types.h"

using base::math::CVector4D;

// Signature as src/game/UnkTU811/fn_7100540290.cpp declares it.
struct UnkMatOwner_b0193;
void fn_7100369508(UnkMatOwner_b0193* self);

__attribute__((visibility("hidden"))) extern const u64 g_unk_7101649138[15];
extern const CVector4D g_unk_71016491b0 __attribute__((visibility("hidden")));

struct UnkBossColourTarget_l3ac9ec {
    u8 pad_0x0[0x138];
    UnkSlots* slots;  // +0x138
};

struct UnkBossColourLinkSub_l3ac9ec {
    u8 pad_0x0[0x50];
    UnkBossColourTarget_l3ac9ec* target;  // +0x50
};

struct UnkBossColourLink_l3ac9ec {
    u8 pad_0x0[0x1b8];
    void* field_0x1b8;
    u8 pad_0x1c0[0x68];
    UnkBossColourLinkSub_l3ac9ec* field_0x228;
};

struct UnkBossColourFlags_l3ac9ec {
    u8 pad_0x0[0x50c];
    bool field_0x50c;
    u8 pad_0x50d[0x3];
    bool field_0x510;
};

struct UnkBossColourEntry_l3ac9ec {
    u32 index;
    u32 id;
    CVector4D value;
};

// The colour slots set to white and the timer that restores them.
struct UnkBossColourState_l3ac9ec {
    f32 timer;
    u32 field_0x4;
    u32 entry_count;
    UnkBossColourEntry_l3ac9ec entries[8];

    void Add(const UnkBossColourEntry_l3ac9ec& entry) { entries[entry_count++] = entry; }
};

struct UnkBossColourAI_l3ac9ec {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkBossColourLink_l3ac9ec* link;  // +0x30
    u8 pad_0x38[0x147d4];
    UnkBossColourState_l3ac9ec colours;  // +0x1480c
    u8 pad_0x148d8[0x30];
    UnkBossColourFlags_l3ac9ec* flags;  // +0x14908
};
static_assert(__builtin_offsetof(UnkBossColourAI_l3ac9ec, colours) == 0x1480c, "layout");
static_assert(__builtin_offsetof(UnkBossColourAI_l3ac9ec, flags) == 0x14908, "layout");

static inline bool Ready(const UnkBossColourLink_l3ac9ec& link, const UnkBossColourTarget_l3ac9ec* target) {
    return (target != nullptr) & (link.field_0x1b8 != nullptr);
}

void fn_71003ac9ec(UnkBossColourAI_l3ac9ec* self) {
    fn_7100369508(reinterpret_cast<UnkMatOwner_b0193*>(self));
    UnkBossColourFlags_l3ac9ec* flags = self->flags;
    if (flags->field_0x50c || !flags->field_0x510)
        return;
    UnkBossColourLink_l3ac9ec* link = self->link;
    UnkBossColourLinkSub_l3ac9ec* sub = link->field_0x228;
    if (!sub)
        return;
    UnkBossColourTarget_l3ac9ec* target = sub->target;
    if (!Ready(*link, target))
        return;
    UnkBossColourState_l3ac9ec& colours = self->colours;
    colours.timer = -1.0f;
    colours.field_0x4 = 0;
    for (u64 i = 0; i < 15; ++i) {
        u32 id = 0xffffffff;
        if (!fn_7100199d88(target->slots->info, &g_unk_7101649138[i], &id))
            continue;
        UnkBossColourEntry_l3ac9ec entry;
        entry.index = i;
        entry.id = id;
        entry.value = {1.0f, 1.0f, 1.0f, 1.0f};
        fn_71001c287c(target->slots, id, 2, &entry.value, 1);
        colours.Add(entry);
        fn_71001c2764(target->slots, entry.id, 1, &g_unk_71016491b0, 1, true);
    }
}
