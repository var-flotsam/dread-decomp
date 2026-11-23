// CProjectileMovement unit (tools/tu_map.py): unless the sound flag at +0x579 is already set, sets it,
// keeps `id` at +0x57c, plays the record at +0x580 at `at` (or the owner's position) through the sound
// manager when the projectile moves, and sets every group of the owner's audio component to `level`.
// Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "game/UnkTU0.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "game/sound/CSoundManager.h"
#include "types.h"

struct UnkAudioComponent_ovb0015;
void fn_710071f634(UnkAudioComponent_ovb0015* self, const u32* groups, s32 level, f32 time);

struct UnkProjectileOwner_lac6540 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x2b8 - 0x68];
    UnkAudioComponent_ovb0015* audio;  // 0x2b8

    UnkVec3f Position() const { return UnkVec3f(x, y, z); }
};

struct UnkProjectileSound_lac6540 {
    u8 pad_0x0[0x30];
    UnkProjectileOwner_lac6540* owner;  // 0x30
    u8 pad_0x38[0x10c - 0x38];
    UnkVec3f velocity;  // 0x10c
    u8 pad_0x118[0x579 - 0x118];
    bool playing;  // 0x579
    u8 pad_0x57a[2];
    u32 id;         // 0x57c
    u8 record[1];  // 0x580
};
static_assert(__builtin_offsetof(UnkProjectileSound_lac6540, record) == 0x580, "record");

// Plays `record` once at `pos`.
inline void PlayAt_lac6540(UnkDropTarget* manager, void* record, const UnkVec3f& pos) {
    fn_7101024b28(manager, record, const_cast<UnkVec3f*>(&pos), nullptr, 0);
}

void fn_7100ac6540(UnkProjectileSound_lac6540* self, const UnkVec3f* at, u32 id, s32 level) {
    if (self->playing)
        return;
    self->playing = true;
    self->id = id;
    f32 speed_sq = LengthSq(self->velocity);
    // Only a moving projectile makes the sound.
    if (!(speed_sq + 0.001f >= 0.0f && speed_sq - 0.001f <= 0.0f))
        PlayAt_lac6540(g_unk_7101d47318, self->record, at ? *at : self->owner->Position());
    if (UnkAudioComponent_ovb0015* audio = self->owner->audio) {
        u32 groups = UnkAll_SpitSend().value;
        fn_710071f634(audio, &groups, level, 0.2f);
    }
}
