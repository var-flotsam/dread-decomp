// CFulmiteBellyMineAIComponent's unit (tools/tu_map.py): once (+0x146f8), hands a kind-5 parameter
// block to the owner's (+0x30) +0x298 (fn_71006f4b0c), removes the id g_unk_7101649f70 from the
// owner's timeline (+0x2e8) and sets g_unk_7101649f88 (when `flag`) or g_unk_7101649f80 to the
// all-flags mask; then, when the actor at +0x146f0 has a CInfesterAIComponent (+0x240), clears its
// +0x146e1. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkBellyMineKey_b60;
extern UnkBellyMineKey_b60 g_unk_7101649f70;  // reached through the GOT
extern UnkBellyMineKey_b60 g_unk_7101649f80;  // reached through the GOT
extern UnkBellyMineKey_b60 g_unk_7101649f88;  // reached through the GOT

struct UnkBellyMineInfesterAI_b60 {
    virtual const base::reflection::CClass* vf_0x0();
    u8 pad_0x8[0x146e1 - 0x8];
    bool field_0x146e1;
};

struct UnkBellyMineInfester_b60 {
    u8 pad_0x0[0x240];
    UnkBellyMineInfesterAI_b60* ai;
};

struct UnkBellyMineOwner_b60 {
    u8 pad_0x0[0x298];
    void* field_0x298;
    u8 pad_0x2a0[0x2e8 - 0x2a0];
    void* field_0x2e8;
};

struct UnkBellyMineAI_b60 {
    u8 pad_0x0[0x30];
    UnkBellyMineOwner_b60* owner;
    u8 pad_0x38[0x146f0 - 0x38];
    UnkBellyMineInfester_b60* infester;
    bool done;
};

// Each path inlines its own mask (so the guarded statics are not merged); an inlined local is placed
// above the parameter block in the frame, as in the original.
static inline __attribute__((always_inline)) void SetAll_b60(UnkTimelineComponent* timeline, UnkBellyMineKey_b60* key) {
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(key), &mask.id, false, -1.0f);
}

void fn_710057df04(UnkBellyMineAI_b60* self, bool flag) {
    if (self->done)
        return;
    UnkParams params;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x8, 0, 13);
    __builtin_memset(params.field_0x20, 0, 16);
    params.field_0x30 = 0;
    params.field_0x34 = 5;
    params.vtable = g_unk_7101a25b10 + 2;
    params.field_0x39 = 0;
    params.field_0x3d = 0;
    *reinterpret_cast<u16*>(&params.pad_0x3a) = 0x101;
    fn_71006f4b0c(self->owner->field_0x298, &params, 0);
    self->done = true;
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        fn_7100a2b7b8(timeline, reinterpret_cast<const u64*>(&g_unk_7101649f70));
        if (flag)
            SetAll_b60(timeline, &g_unk_7101649f88);
        else
            SetAll_b60(timeline, &g_unk_7101649f80);
    }
    UnkBellyMineInfesterAI_b60* ai = self->infester->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71005ca678()))
        ai->field_0x146e1 = false;
}
