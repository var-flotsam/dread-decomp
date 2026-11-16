// CScorpiusAIComponent's unit (tools/tu_map.py): ends a phase of the fight. Optionally clears its
// flags and two of the owner's switches, quiets the owner's audio, fades the music when asked, and
// once the phase's timeline id is pending sets it, takes the owner out of the game's list and sets
// another; finally marks the owner's part as changed. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CRntVector.h"
#include "game/CAudioComponent.h"
#include "game/CBossAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU218.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct UnkScorpiusKey_b59a4;
extern UnkScorpiusKey_b59a4 g_unk_710164a560;  // reached through the GOT
extern UnkScorpiusKey_b59a4 g_unk_710164a608;  // reached through the GOT
extern UnkScorpiusKey_b59a4 g_unk_710164a548;  // reached through the GOT

// An entry of the game's list: two words compared and copied together.
struct UnkListEntry_l692c8c {
    u32 lo;
    u32 hi;

    UnkListEntry_l692c8c() : lo(0), hi(0) {}
    bool operator==(const UnkListEntry_l692c8c& other) const { return lo == other.lo && hi == other.hi; }
};

// A fixed-capacity list of entries with its count in front.
struct UnkEntryList_l692c8c {
    u8 pad_0x0[0x848];
    u32 count;
    UnkListEntry_l692c8c entries[1];

    void Resize(u32 n) {
        for (u32 i = count; i < n; i++)
            entries[i] = UnkListEntry_l692c8c();
        count = n;
    }
    void Remove(const UnkListEntry_l692c8c& entry) {
        UnkListEntry_l692c8c* end = &entries[count];
        UnkListEntry_l692c8c* it = entries;
        for (; it != end; ++it) {
            if (*it == entry)
                break;
        }
        if (it == end)
            return;
        for (UnkListEntry_l692c8c* next = it + 1; next != &entries[count]; ++next)
            next[-1] = *next;
        Resize(count - 1);
    }
};

struct UnkGameOwner_l692c8c {
    u8 pad_0x0[0x2b0];
    UnkEntryList_l692c8c* list;
};

struct UnkScorpiusPart_l692c8c {
    u8 pad_0x0[0x78];
    void* switches;
    u8 pad_0x80[0x2dc];
    bool changed;  // 0x35c
};

struct UnkScorpiusParts_l692c8c {
    u8 pad_0x0[0x138];
    UnkScorpiusPart_l692c8c* part;
};

struct UnkScorpiusModel_l692c8c {
    u8 pad_0x0[0x50];
    UnkScorpiusParts_l692c8c* parts;
};

struct UnkScorpiusOwner_l692c8c {
    u8 pad_0x0[0x228];
    UnkScorpiusModel_l692c8c* model;
    u8 pad_0x230[0x70];
    UnkListEntry_l692c8c entry;  // 0x2a0
    u8 pad_0x2a8[0x10];
    void* audio;  // 0x2b8
    u8 pad_0x2c0[0x28];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkScorpiusAI_l692c8c {
    u8 pad_0x0[0x30];
    UnkScorpiusOwner_l692c8c* owner;
    u8 pad_0x38[0x14c28 - 0x38];
    bool field_0x14c28;
    bool timeline_pending;  // 0x14c29
    u8 pad_0x14c2a[0x9a];
    u32 field_0x14cc4;
    u8 pad_0x14cc8[0xb8];
    bool fade_music;  // 0x14d80
};
static_assert(__builtin_offsetof(UnkScorpiusAI_l692c8c, field_0x14cc4) == 0x14cc4, "layout");
static_assert(__builtin_offsetof(UnkScorpiusAI_l692c8c, fade_music) == 0x14d80, "layout");

static inline UnkMgr_0xf18* MusicManager() {
    UnkMgr_0xf18* manager = g_unk_71726bc560;
    if (!manager) {
        manager = static_cast<UnkMgr_0xf18*>(fn_71000839a0(0xf18, 0x3f));
        fn_7100fd3804(manager);
        g_unk_71726bc560 = manager;
    }
    return manager;
}

void fn_7100692c8c(UnkScorpiusAI_l692c8c* self, bool reset, bool alternate) {
    if (reset) {
        self->field_0x14c28 = false;
        if (UnkScorpiusParts_l692c8c* parts = self->owner->model->parts) {
            u64 first = 0x771d79f0dfb117cf;
            u64 second = 0xbdf1cd4cf595f96b;
            fn_7100199420(parts->part->switches, &first, false);
            fn_7100199420(parts->part->switches, &second, true);
        }
        self->field_0x14cc4 = 0;
    }
    void* audio = self->owner->audio;
    if (!audio)
        goto music;
    // Jumped over: no lifetime markers, so each keeps its own slot.
    base::reflection::UnkReflTypeIdArg audioMask;
    base::reflection::UnkReflTypeIdArg audioMask2;
    audioMask.id = 1;
    fn_710071a398(audio, &audioMask.id, 1.0f, 0.0f);
    audioMask2.id = 0x40;
    fn_710071a398(audio, &audioMask2.id, 0.0f, 0.0f);
music:
    if (self->fade_music) {
        fn_7100fdcf2c(MusicManager(), 0, 0, 0.2f);
        fn_7100fdcf2c(MusicManager(), 1, 1, 0.2f);
    }
    if (self->timeline_pending) {
        self->timeline_pending = false;
        if (UnkTimelineComponent* timeline = self->owner->timeline) {
            if (alternate) {
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a608), &mask.id, false,
                              -1.0f);
            } else {
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a548), &mask.id, false,
                              -1.0f);
            }
        }
        if (auto* owner = reinterpret_cast<UnkGameOwner_l692c8c*>(fn_7100defcd4(g_unk_7101d47310))) {
            if (UnkEntryList_l692c8c* list = owner->list)
                list->Remove(self->owner->entry);
        }
        fn_7100a2b7b8(self->owner->timeline, reinterpret_cast<const u64*>(&g_unk_710164a560));
    }
    if (UnkScorpiusModel_l692c8c* model = self->owner->model) {
        if (UnkScorpiusParts_l692c8c* parts = model->parts)
            parts->part->changed = true;
    }
}
