// CProjectileMovement unit (tools/tu_map.py): when the sound flag at +0x579 is set, clears it and the
// word at +0x57c, plays the record at +0x580 at `at` (or the owner's position) through the sound
// manager, then fades every group of the owner's audio component. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "game/UnkTU0.h"
#include "game/UnkXformNode.h"
#include "game/sound/CSoundManager.h"
#include "types.h"

struct UnkAudioComponent_ovb0015;
void fn_710071f634(UnkAudioComponent_ovb0015* self, const u32* groups, s32 level, f32 time);

struct UnkProjectileOwner_lac6704 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x2b8 - 0x68];
    UnkAudioComponent_ovb0015* audio;

    UnkVec3f Position() const { return UnkVec3f(x, y, z); }
};

struct UnkProjectileSound_lac6704 {
    u8 pad_0x0[0x30];
    UnkProjectileOwner_lac6704* owner;
    u8 pad_0x38[0x579 - 0x38];
    bool playing;
    u8 pad_0x57a[2];
    u32 field_0x57c;
    u8 record[1];
};
static_assert(__builtin_offsetof(UnkProjectileSound_lac6704, record) == 0x580, "record");

// Plays `record` once at `pos`.
inline void PlayAt_lac6704(UnkDropTarget* manager, void* record, const UnkVec3f& pos) {
    fn_7101024b28(manager, record, const_cast<UnkVec3f*>(&pos), nullptr, 0);
}

void fn_7100ac6704(UnkProjectileSound_lac6704* self, const UnkVec3f* at) {
    if (!self->playing)
        return;
    self->playing = false;
    self->field_0x57c = 0;
    PlayAt_lac6704(g_unk_7101d47318, self->record, at ? *at : self->owner->Position());
    if (UnkAudioComponent_ovb0015* audio = self->owner->audio) {
        u32 groups = UnkAll_SpitSend().value;
        fn_710071f634(audio, &groups, 0, 0.2f);
    }
}
