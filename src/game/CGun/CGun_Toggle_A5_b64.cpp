// CGun's unit: switches the gun's timeline entry. The source id defaults to the empty id and is taken from
// the track record when the owner (+0x18) plays a CCharClassAddWeaponTimelineSuffixTrack. Nothing happens
// while the id at +0x50 is empty. With the argument's flag (+4) set and its mode (+0) not 2, the entry at
// +0x60 is set to the all-flags mask when the owner's timeline knows it; otherwise the timeline entry (+0x50
// or, with the toggle at +0x4c set, +0x58) is replaced under the source id and the toggle flips.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;

void fn_7100a2cddc(void* table, void* key, CStrId* id, f32 delay, bool unique);
bool fn_7100a2caac(void* table, CStrId* key, CStrId* id);
void fn_7100a2d400(void* table, CStrId* key, CStrId* id);

struct UnkGunToggleArg_A5_b64 {
    u32 mode;
    bool flag;
};

struct UnkGunToggle_A5_b64 {
    u8 pad_0x0[0x18];
    UnkActor* owner;
    u8 pad_0x20[0x2c];
    bool toggle;  // 0x4c
    u8 pad_0x4d[3];
    CStrId first;   // 0x50
    CStrId second;  // 0x58
    u64 key;        // 0x60
};

void fn_7100835f94(UnkGunToggle_A5_b64* self, UnkGunToggleArg_A5_b64* arg) {
    CStrId source(g_unk_7101d470e0);
    UnkTrackQuery query;
    query.cls = fn_7100beff48();
    query.result = nullptr;
    query.value = -1.0f;
    if (fn_710117b08c(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), &query) && query.result)
        source = query.result->sources;
    if (self->first.data == &g_unk_7101d47108->empty)
        return;
    if (arg->flag) {
        if (arg->mode == 2)
            return;
        auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
        if (!timeline)
            return;
        if (!fn_7100a2c13c(timeline, &self->key))
            return;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &self->key, &mask.id, false, -1.0f);
    } else {
        void* timeline = self->owner->field_0x2e8;
        if (!timeline)
            return;
        CStrId entry(self->toggle ? self->second : self->first);
        if (fn_7100a2caac(timeline, &entry, &source)) {
            fn_7100a2d400(timeline, &entry, &source);
            fn_7100a2cddc(timeline, &entry, &source, -1.0f, false);
        }
        self->toggle = !self->toggle;
    }
}
