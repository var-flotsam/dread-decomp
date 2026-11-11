// CAbilityEnergyWave's unit (tools/tu_map.py): once the timer at +0xe0 reaches the tunable's limit (+0x54 of
// g_unk_71726af8b0) the key g_unk_7101648cf8 is set on the owner's (+0x30) timeline (+0x2e8) to the all-flags mask;
// the timer is cleared and the multi-lockon entry (fn_71003182d8, found by fn_7100300970) is told (slot 0xa0) to
// stop when its state is 6 (state at +0xb4, or while that is 7 the state of its object at +0x20). Names are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityMultiLockon.h"
#include "game/UnkActor.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

using base::global::CStrId;

struct UnkEnergyWaveKey_b60a2;
extern UnkEnergyWaveKey_b60a2 g_unk_7101648cf8;  // reached through the GOT
extern UnkTunable g_unk_71726af8b0;
struct UnkEntry;
struct UnkOwner;
UnkEntry* fn_7100300970(UnkOwner* self, const CStrId* id);

struct UnkWaveSubState_b62a2 {
    u8 pad_0x0[0x10];
    s32 state;
};
struct UnkWaveEntry_b62a2 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0(bool);
    u8 pad_0x8[0x18];
    UnkWaveSubState_b62a2* sub;
    u8 pad_0x28[0x8c];
    s32 state;
};

struct UnkEnergyWave_b62a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0xe0 - 0x38];
    f32 timer;
};

void fn_71002fe318(UnkEnergyWave_b62a2* self) {
    if (self->timer >= *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_71726af8b0) + 0x54)) {
        auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101648cf8), &mask.id, false, -1.0f);
    }
    self->timer = 0.0f;
    u64 id = fn_71003182d8();
    auto* entry = reinterpret_cast<UnkWaveEntry_b62a2*>(
        fn_7100300970(reinterpret_cast<UnkOwner*>(self), reinterpret_cast<const CStrId*>(id)));
    if (!entry)
        return;
    s32 state = entry->state;
    if (state == 7) {
        if (!entry->sub)
            return;
        state = entry->sub->state;
    }
    if (state == 6)
        entry->vf_0xa0(true);
}
