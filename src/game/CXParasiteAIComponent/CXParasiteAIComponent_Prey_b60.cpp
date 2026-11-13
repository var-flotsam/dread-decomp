// CXParasiteAIComponent's unit (tools/tu_map.py): takes a new prey. For a non-null `prey`, the counted
// weak handle at +0x14798 is pointed at it (fn_7100d78378, AssignRef as
// src/game/CProjectileMovement/CProjectileMovement_SetTarget_b46.cpp); the owner's (+0x30) +0x220 object is
// sent the hash at +0x30 of its +0x2b8 object with mode 2 (as src/game/CAttack/CAttack_Send_b55.cpp), the
// owner's timeline (+0x2e8) gets g_unk_710164a840 set to the all-flags mask, and the value at +0x147a8 is
// the prey's value at +0x37c, or fn_7100977b20's when the prey has its +0x218 object. Names and layouts
// are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkXParasiteAI.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkXParasitePreyKey_b60a4;
extern UnkXParasitePreyKey_b60a4 g_unk_710164a840;  // reached through the GOT

struct UnkXParasitePreyName_b60a4 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkXParasitePreySink_b60a4 {
    u8 pad_0x0[0x2b8];
    UnkXParasitePreyName_b60a4* field_0x2b8;
};

struct UnkXParasitePrey_b60a4 {
    u8 pad_0x0[0x218];
    void* field_0x218;
    u8 pad_0x220[0x37c - 0x220];
    f32 field_0x37c;
};

void fn_71006ea9a4(UnkXParasiteAI* self, UnkWeakTarget* prey) {
    if (!prey)
        return;
    // Declared past a goto (no lifetime markers), so the three keep their own slots.
    goto start;
    base::reflection::UnkReflTypeIdArg timeline_mask;
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
start:
    AssignRef(&self->field_0x14798, fn_7100d78378(prey));
    if (auto* sink = static_cast<UnkXParasitePreySink_b60a4*>(self->owner->field_0x220)) {
        u64 hash = sink->field_0x2b8->hash;
        mode.id = 2;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(sink, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    }
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        timeline_mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a840), &timeline_mask.id, false, -1.0f);
    }
    UnkXParasitePrey_b60a4* target = reinterpret_cast<UnkXParasitePrey_b60a4*>(self->field_0x14798->target);
    if (!target->field_0x218)
        self->field_0x147a8 = target->field_0x37c;
    else
        self->field_0x147a8 = fn_7100977b20(target->field_0x218);
}
