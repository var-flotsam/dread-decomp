// CAIManager::CTunableAIManager's unit: destructor of the record fn_7100dac280 builds (vtable
// g_unk_7101aa9328). Fades out its two sounds, lifts the sound manager's mutes it set, then drops its
// counted references. Names and layouts are placeholders.
#include "base/snd/CSoundSystem.h"
#include "base/snd/UnkSound.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/UnkTU101.h"
#include "game/UnkTU23.h"
#include "game/UnkTU39.h"
#include "game/sound/UnkSoundHandle.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_7100108be8(UnkSoundMgr* self, const u32* mask, f32 time);
void fn_7100108c24(UnkSoundMgr* self, u32 group);

// As src/game/CAudioComponent/CAudioComponent_b50.cpp defines them.
struct UnkSoundObject_b50;
struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};

// A counted reference kept 4-aligned in the record's small array.
struct UnkPackedRef_dac2e4 {
    UnkRefObject* p;
} __attribute__((packed, aligned(4)));

struct UnkAIRecord_dac2e4 {
    void* vtable;
    u32 count;
    UnkPackedRef_dac2e4 items[2];
    u8 unk_0x1c[0x20 - 0x1c];
    UnkSoundHandle sound_0x20;
    UnkSoundHandle sound_0x30;
    u8 unk_0x40[0x45 - 0x40];
    bool muted_0x45;
    bool muted_0x46;
    u8 unk_0x47[0x48 - 0x47];
    UnkRefObject* field_0x48;
    UnkRefObject* field_0x50;
    UnkRefObject* field_0x58;
    u8 unk_0x60[0x68 - 0x60];
    u8 field_0x68[0x118 - 0x68];
    UnkRefObject* field_0x118;
    u8 unk_0x120[0x128 - 0x120];
    UnkRefObject* field_0x128;
    UnkRefObject* field_0x130;
};
static_assert(__builtin_offsetof(UnkAIRecord_dac2e4, field_0x130) == 0x130, "layout");

// One counted reference less; the last one deletes the object (vtable slot 0x10).
static inline void Release(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

static inline void FadeOut(UnkSoundHandle& handle) {
    if (handle.ref && handle.ref->serial == handle.serial && handle.serial != -1) {
        if (UnkSound* sound = reinterpret_cast<UnkSound*>(handle.ref->object)) {
            fn_7100105a80(sound, 0.2f, 0.0f);
            handle = g_unk_7108b6c460;
        }
    }
}

void fn_7100dac2e4(UnkAIRecord_dac2e4* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101aa9328) + 0x10;
    FadeOut(self->sound_0x20);
    FadeOut(self->sound_0x30);

    UnkSoundMgr*& sound = g_unk_7101d4d820;
    alignas(8) u32 duck_mask;
    alignas(8) u32 mute_mask;
    if (self->muted_0x45) {
        self->muted_0x45 = false;
        mute_mask = 0x41;
        fn_7100108be8(sound, &mute_mask, 0.0f);
    }
    fn_7100108c24(sound, 6);
    if (self->muted_0x46) {
        self->muted_0x46 = false;
        duck_mask = 0x40;
        fn_7100108c2c(sound, &duck_mask, 0, 0.2f);
    }

    Release(self->field_0x130);
    Release(self->field_0x128);
    Release(self->field_0x118);
    fn_710002f67c(self->field_0x68);
    Release(self->field_0x58);
    Release(self->field_0x50);
    Release(self->field_0x48);
    for (u64 i = 0; i < self->count; ++i)
        Release(self->items[i].p);
    self->count = 0;
}
