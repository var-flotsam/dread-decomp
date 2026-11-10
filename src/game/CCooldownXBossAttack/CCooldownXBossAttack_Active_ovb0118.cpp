// CCooldownXBossAttack's unit (tools/tu_map.py): fn_71004c1f18 and fn_71004c1e08 set and clear the
// flag at +0x60 and, when it changes, set the timeline id of the attack's two tracks (the hashes at
// +0x30 of the +0xf0 and +0xf8 entries of the attack's definition at +0x28) on the owner's timeline
// (+0x2e8) to the all-flags mask. Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkCooldownTrack_ovb0118 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkCooldownAttackDef_ovb0118 {
    u8 pad_0x0[0xf0];
    UnkCooldownTrack_ovb0118* start_track;  // 0xf0
    UnkCooldownTrack_ovb0118* end_track;    // 0xf8
};

struct UnkCooldownOwner_ovb0118 {
    u8 pad_0x0[0x2e8];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkCooldownAttack_ovb0118 {
    u8 pad_0x0[0x8];
    UnkCooldownOwner_ovb0118* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkCooldownAttackDef_ovb0118* def;  // 0x28
    u8 pad_0x30[0x30];
    bool active;  // 0x60
};

// Sets the track's timeline id to the all-flags mask.
static inline void SetTrack(UnkTimelineComponent* timeline, u64 track) {
    alignas(8) u32 mask;
    u64 id = track;
    mask = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
    fn_7100a2cb7c(timeline, &id, &mask, false, -1.0f);
}

// Typed as src/game/CCooldownXBossLaserBiteAttack/CCooldownXBossLaserBiteAttack_Update_b59.cpp declares them.
struct UnkLaserBite_b59;

void fn_71004c1e08(UnkLaserBite_b59* attack) {
    UnkCooldownAttack_ovb0118* self = reinterpret_cast<UnkCooldownAttack_ovb0118*>(attack);
    if (!self->active)
        return;
    self->active = false;
    SetTrack(self->owner->timeline, self->def->end_track->id);
}

void fn_71004c1f18(UnkLaserBite_b59* attack) {
    UnkCooldownAttack_ovb0118* self = reinterpret_cast<UnkCooldownAttack_ovb0118*>(attack);
    if (self->active)
        return;
    self->active = true;
    SetTrack(self->owner->timeline, self->def->start_track->id);
}
