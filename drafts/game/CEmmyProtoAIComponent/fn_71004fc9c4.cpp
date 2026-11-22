// CEmmyProtoAIComponent's unit (tools/tu_map.py): ends the near-chase scratch loop. With a character class
// and a target, it plays "ncu_chasenear-scratchloop_end" when the owner already faces the target's side,
// else the "_turn" variant, starting it from what is left of the current track's first 12 units.
// Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CName.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

// A time on a track.
struct UnkTrackTime_l4fc9c4 {
    f32 value;
};

// The start the track is played from (a 12-byte value).
struct UnkTrackStart_l4fc9c4 {
    u64 field_0x0;
    UnkTrackTime_l4fc9c4 field_0x8;

    explicit UnkTrackStart_l4fc9c4(UnkTrackTime_l4fc9c4 time) : field_0x0(0), field_0x8(time) {}
};

struct UnkEmmyProtoAI_l4fc9c4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

static const u64 kTrackKind = 0xf4061ff25a2163d6;

bool fn_71004fc9c4(UnkEmmyProtoAI_l4fc9c4* self) {
    if (!self->owner->field_0x220)
        return false;
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return false;

    UnkTrackTime_l4fc9c4 start;
    start.value = 0.0f;
    auto* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (fn_710117ae38(animation, 0xe48330c64dc6bb08, false)) {
        f32 t = fn_7101177ea0(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), kTrackKind);
        start.value = !(t <= 12.0f) ? 0.0f : 12.0f - t;
    }

    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    f32 dx = target->field_0x44 - self->owner->field_0x44;
    f32 side = (dx > 0.0f) - (dx < 0.0f);
    f32 facing = m.m[2];
    void* character = self->owner->field_0x220;
    base::reflection::UnkReflTypeIdArg mask;
    if (facing + 0.001f >= side && facing - 0.001f <= side) {
        u64 hash = base::global::CName("ncu_chasenear-scratchloop_end").hash;
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 0;
        UnkTrackStart_l4fc9c4 from(start);
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(character, hash, kTrackKind, &mode, &g_unk_710164b958, &from, &mask);
    } else {
        u64 hash = base::global::CName("ncu_chasenear-scratchloop_end_turn").hash;
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 0;
        UnkTrackStart_l4fc9c4 from(start);
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(character, hash, kTrackKind, &mode, &g_unk_710164b958, &from, &mask);
    }
    return true;
}
