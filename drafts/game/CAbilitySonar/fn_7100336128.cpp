// CAbilitySonar's unit (tools/tu_map.py unit 434): updates the tracked targets from the ones the pulse found:
// lost ones are told (vf_0x20), new ones too (vf_0x18), up to 32 are kept and asked whether they react
// (vf_0x8), the rest are told they were dropped (vf_0x28); then the two sounds fade by the result.
// Names and layouts are placeholders.
#include <string.h>

#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU101.h"
#include "types.h"

extern base::reflection::UnkTunable g_unk_71726b0c48;

struct UnkSonarTunable_l336128 {
    u8 pad_0x0[0x64];
    f32 fade_time;  // 0x64
};

struct UnkSonarPos_l336128 {
    f32 x;
    f32 y;
};

class UnkSonarTarget_l336128 {
public:
    virtual void vf_0x0();
    virtual bool vf_0x8(const UnkSonarPos_l336128* pos, f32 z);
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
};

// Up to 32 targets after their count (unaligned storage), grown on access.
struct UnkSonarTargetSet_l336128 {
    u32 count;
    u8 storage[32 * sizeof(UnkSonarTarget_l336128*)];

    UnkSonarTarget_l336128** items() { return reinterpret_cast<UnkSonarTarget_l336128**>(storage); }
    UnkSonarTarget_l336128*& At(u32 i) {
        if (i >= count) {
            memset(&items()[count], 0, (i + 1 - count) * sizeof(UnkSonarTarget_l336128*));
            count = i + 1;
        }
        return items()[i];
    }
};

struct UnkSonarSoundHandle_l336128 {
    UnkPlayingSound_ovb0074* sound;
    u64 id;
};

struct UnkSonarSoundRef_l336128 {
    UnkSonarSoundHandle_l336128* handle;
    u64 handle_id;  // -1 for none

    UnkPlayingSound_ovb0074* Get() const {
        if (handle && handle->id == handle_id && handle_id != (u64)-1)
            return handle->sound;
        return nullptr;
    }
};

struct UnkSonarOwnerData_l336128 {
    u8 pad_0x0[0x64];
    f32 z;  // 0x64
};
struct UnkSonarOwner_l336128 {
    u8 pad_0x0[0x30];
    UnkSonarOwnerData_l336128* data;  // 0x30
};

struct UnkAbilitySonar_l336128 {
    u8 pad_0x0[0xc0];
    UnkSonarOwner_l336128* owner;  // 0xc0
    u8 pad_0xc8[0x6e0 - 0xc8];
    UnkSonarPos_l336128 pos;  // 0x6e0
    u8 pad_0x6e8[0x758 - 0x6e8];
    bool reacting;  // 0x758
    u8 pad_0x759[0x790 - 0x759];
    UnkSonarSoundRef_l336128 sound_a;  // 0x790
    UnkSonarSoundRef_l336128 sound_b;  // 0x7a0
};
static_assert(__builtin_offsetof(UnkAbilitySonar_l336128, pos) == 0x6e0, "layout");
static_assert(__builtin_offsetof(UnkAbilitySonar_l336128, reacting) == 0x758, "layout");
static_assert(__builtin_offsetof(UnkAbilitySonar_l336128, sound_b) == 0x7a0, "layout");

void fn_7100336128(UnkAbilitySonar_l336128* self, UnkSonarTargetSet_l336128* tracked,
                   UnkSonarTargetSet_l336128* found) {
    bool still_found[64] = {};
    u32 tracked_before = tracked->count;

    // Targets no longer found are told so.
    for (u32 i = 0; i < tracked->count; ++i) {
        bool lost = true;
        for (u32 j = 0; j < found->count; ++j) {
            if (tracked->At(i) == found->items()[j]) {
                still_found[j] = true;
                lost = false;
                break;
            }
        }
        if (lost)
            tracked->At(i)->vf_0x20();
    }

    // Keep up to 32 of the found ones; new ones are told so.
    tracked->count = 0;
    bool reacting = false;
    for (u32 j = 0; j < found->count && tracked->count < 32; ++j) {
        UnkSonarTarget_l336128* target = found->items()[j];
        if (!still_found[j])
            target->vf_0x18();
        reacting = reacting || target->vf_0x8(&self->pos, self->owner->data->z);
        tracked->items()[tracked->count] = target;
        ++tracked->count;
    }
    for (u32 k = tracked->count; k < found->count; ++k)
        found->items()[k]->vf_0x28();

    f32 fade = reinterpret_cast<const UnkSonarTunable_l336128*>(&g_unk_71726b0c48)->fade_time;
    if (tracked->count == 0) {
        if (UnkPlayingSound_ovb0074* s = self->sound_a.Get())
            fn_7100105c9c(s, fade);
        if (UnkPlayingSound_ovb0074* s = self->sound_b.Get())
            fn_7100105c9c(s, fade);
    } else if (!reacting & (self->reacting | (tracked_before == 0))) {
        if (UnkPlayingSound_ovb0074* s = self->sound_a.Get())
            fn_7100105c9c(s, fade);
        if (UnkPlayingSound_ovb0074* s = self->sound_b.Get())
            fn_7100105cf0(s, fade);
    } else if (reacting & !self->reacting) {
        if (UnkPlayingSound_ovb0074* s = self->sound_a.Get())
            fn_7100105cf0(s, fade);
        if (UnkPlayingSound_ovb0074* s = self->sound_b.Get())
            fn_7100105c9c(s, fade);
    }
    self->reacting = reacting;
}
