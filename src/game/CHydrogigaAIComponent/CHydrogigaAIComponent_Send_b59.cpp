// The CHydrogigaAIComponent unit. fn_71005c2500 clears the word at +0x149d0, then, when the owner (+0x30)
// has its collision container (+0x2a0), turns off every collider found under g_unk_71726eb438
// (fn_71008651c8 into a local list) and turns on the one named g_unk_71726eb440; sends the owner's
// sub-object at +0x220 a one under id 0x3bef2eb1f76bd18c for two kinds (all-flags mask, then the empty one)
// and clears +0x149c4; finally sets the timeline (+0x2e8) id g_unk_710164a0e0 to the all-flags mask. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkVector.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

// As in src/game/CCollisionComponent/fn_71008651c8.cpp.
struct UnkStruct_71008651c8_Entry;
struct UnkStruct_71008651c8;
using UnkOut_71008651c8 = base::UnkVector<UnkStruct_71008651c8_Entry*>;
void fn_71008651c8(UnkStruct_71008651c8* self, void* const* key, UnkOut_71008651c8* out);

extern void* const g_unk_71726eb438 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_71726eb440 __attribute__((visibility("hidden")));
struct UnkTimelineKey_b59a3;
extern UnkTimelineKey_b59a3 g_unk_710164a0e0;  // reached through the GOT

struct UnkHydrogigaOwner_b59a3 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x2a0 - 0x228];
    UnkCollisionContainer* field_0x2a0;
    u8 pad_0x2a8[0x2e8 - 0x2a8];
    UnkTimelineComponent* field_0x2e8;
};

struct UnkHydrogigaAI_b59a3 {
    u8 pad_0x0[0x30];
    UnkHydrogigaOwner_b59a3* owner;
    u8 pad_0x38[0x149c4 - 0x38];
    u32 field_0x149c4;
    u8 pad_0x149c8[0x8];
    u32 field_0x149d0;
};

void fn_71005c2500(UnkHydrogigaAI_b59a3* self) {
    self->field_0x149d0 = 0;
    UnkCollisionContainer* container = self->owner->field_0x2a0;
    void* target;
    if (!container)
        goto send;
    base::reflection::UnkReflTypeIdArg mode1;
    {
        UnkOut_71008651c8 found(0xf);
        fn_71008651c8(reinterpret_cast<UnkStruct_71008651c8*>(container), &g_unk_71726eb438, &found);
        for (UnkStruct_71008651c8_Entry **it = found.data, **end = found.data + found.count; it != end; ++it)
            fn_7100d35a98(*reinterpret_cast<UnkEntry**>(reinterpret_cast<char*>(*it) + 0x28), false);
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(fn_71008640a4(container, &g_unk_71726eb440)), true);
        found.Free();
    }
send:
    target = self->owner->field_0x220;
    // The goto skips the declarations below, so they get no lifetime markers and keep slots of their own.
    if (!target)
        goto timeline;
    base::reflection::UnkReflTypeIdArg mask1;
    base::reflection::UnkReflTypeIdArg mode2;
    base::reflection::UnkReflTypeIdArg mask2;
    mode1.id = 1;
    mask1.id = UnkAll_SpitSend().value;
    fn_7101176690(target, 0x3bef2eb1f76bd18c, 0xf4061ff25a2163d6, &mode1, &g_unk_710164b958, &g_unk_7172769740,
                  &mask1);
    mode2.id = 1;
    mask2.id = UnkNone_SpitSend().value;
    fn_7101176690(target, 0x3bef2eb1f76bd18c, 0xbd794f4bf6d1d598, &mode2, &g_unk_710164b958, &g_unk_7172769740,
                  &mask2);
    self->field_0x149c4 = 0;
timeline:
    if (UnkTimelineComponent* timeline = self->owner->field_0x2e8) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a0e0), &mask.id, false, -1.0f);
    }
}
