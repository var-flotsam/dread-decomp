// CScorpiusAIComponent unit: true when none of six timeline ids is playing and the progress value at +0x14c2c
// sits within 0.001 of the target its state asks for (0 unless the state is 2 or 3), and, while the
// owner is below the tunable at +0x40 of g_unk_7172703e88, only when the owner's animation is in a
// CCharClassScorpiusAvoidMediumHeadPositionTrack and the state is 2. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CCharClassScorpiusAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern const u64 g_unk_710164a5e0;
extern const u64 g_unk_710164a5d8;
extern const u64 g_unk_710164a5d0;
extern const u64 g_unk_710164a5c8;
extern const u64 g_unk_710164a5c0;
extern const u64 g_unk_710164a5b8;

struct UnkScorpiusTunables_l696570 {
    u8 pad_0x0[0x40];
    f32 field_0x40;
};

struct UnkScorpiusProgress_l696570 {
    f32 value;      // 0x0
    u32 state;      // 0x4
    u8 pad_0x8[0x84];
    f32 elapsed;    // 0x8c
};

struct UnkScorpiusOwner_l696570 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0xc0];
    UnkTimelineComponent* timeline;    // 0x2e8
};

struct UnkScorpiusAI_l696570 {
    u8 pad_0x0[0x30];
    UnkScorpiusOwner_l696570* owner;  // 0x30
    u8 pad_0x38[0x14c2c - 0x38];
    UnkScorpiusProgress_l696570 progress;  // 0x14c2c
};

bool fn_7100696570(UnkScorpiusAI_l696570* self) {
    UnkScorpiusOwner_l696570* owner = self->owner;
    if (UnkTimelineComponent* timeline = owner->timeline) {
        if (fn_7100a2be94(timeline, &g_unk_710164a5e0) || fn_7100a2be94(timeline, &g_unk_710164a5d8) ||
            fn_7100a2be94(timeline, &g_unk_710164a5d0) || fn_7100a2be94(timeline, &g_unk_710164a5c8) ||
            fn_7100a2be94(timeline, &g_unk_710164a5c0) || fn_7100a2be94(timeline, &g_unk_710164a5b8))
            return false;
        owner = self->owner;
    }
    UnkScorpiusProgress_l696570* p = &self->progress;
    const UnkScorpiusTunables_l696570* tunables = reinterpret_cast<const UnkScorpiusTunables_l696570*>(&g_unk_7172703e88);
    if (!(p->elapsed >= tunables->field_0x40)) {
        UnkAnimationComponent* animation = owner->animation;
        if (!animation)
            return false;
        if (!fn_710117b348(animation, fn_7100c4ad78()))
            return false;
        if (p->state != 2)
            return false;
    }
    f32 target = 0.0f;
    u32 state = p->state;
    if (state == 2) {
        target = 0.5f;
        state = 2;
    }
    if (state == 3)
        target = 1.0f;
    return p->value + 0.001f >= target && p->value - 0.001f <= target;
}
