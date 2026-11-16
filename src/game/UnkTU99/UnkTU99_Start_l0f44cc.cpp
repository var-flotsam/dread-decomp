// Unit 99 (base::snd::CSoundSystemATK): starts a voice on a new command list. A voice that is still
// playing is stopped first (its sound and player, fade 2); then the player is pointed at the target, the
// command's entries are copied in and playback restarts at the first entry. Names and layouts are placeholders.
#include <new>

#include "game/UnkTU2772.h"
#include "types.h"

struct UnkSoundCommand;
// nn::atk (library): stopping a sound and a player, with a fade of `frames`, and releasing them.
void fn_7101209300(void** handle);
void fn_7101206578(u8* player);
void fn_7101206444(u8* player, void* target);

namespace {

#pragma pack(push, 4)
struct UnkSoundParams_l0f44cc {
    u32 id;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
};

struct UnkSoundEntry_l0f44cc {
    UnkSoundParams_l0f44cc params;
    u32 field_0x1c;

    UnkSoundEntry_l0f44cc(const UnkSoundEntry_l0f44cc&) = default;
    // Assignment takes only the parameters; field_0x1c stays as it was.
    UnkSoundEntry_l0f44cc& operator=(const UnkSoundEntry_l0f44cc& other) {
        params = other.params;
        return *this;
    }
};
#pragma pack(pop)

// A list of up to eight entries with its count in front.
struct UnkSoundEntries_l0f44cc {
    u32 count;
    // Storage for the entries; only the first `count` are constructed.
    u8 storage[8 * sizeof(UnkSoundEntry_l0f44cc)];

    UnkSoundEntry_l0f44cc* Data() { return reinterpret_cast<UnkSoundEntry_l0f44cc*>(storage); }
    const UnkSoundEntry_l0f44cc* Data() const {
        return reinterpret_cast<const UnkSoundEntry_l0f44cc*>(storage);
    }

    void PushBack(const UnkSoundEntry_l0f44cc& entry) {
        new (&Data()[count]) UnkSoundEntry_l0f44cc(entry);
        ++count;
    }

    UnkSoundEntries_l0f44cc& operator=(const UnkSoundEntries_l0f44cc& other) {
        if (this != &other) {
            u32 n = count;
            if (n > other.count) {
                for (u32 i = 0; i < other.count; ++i)
                    Data()[i] = other.Data()[i];
            } else {
                for (u32 i = 0; i < n; ++i)
                    Data()[i] = other.Data()[i];
            }
            for (; n < other.count; ++n)
                PushBack(other.Data()[n]);
            count = other.count;
        }
        return *this;
    }
};

struct UnkSoundVoice_l0f44cc {
    u8 pad_0x0[0x28];
    void** handle;
    u8* player;
    UnkSoundEntries_l0f44cc entries;
    s32 entry;
    u8 pad_0x140[0x2c];
    bool playing;
    bool field_0x16d;
    bool field_0x16e;
    bool field_0x16f;
    bool field_0x170;
};
static_assert(__builtin_offsetof(UnkSoundVoice_l0f44cc, entry) == 0x13c, "");
static_assert(__builtin_offsetof(UnkSoundVoice_l0f44cc, playing) == 0x16c, "");

}  // namespace

void fn_71000f44cc(void* self, void* target, UnkSoundCommand* command) {
    UnkSoundVoice_l0f44cc* voice = static_cast<UnkSoundVoice_l0f44cc*>(self);
    if (voice->playing) {
        voice->playing = false;
        voice->entries.count = 0;
        voice->entry = -1;
        if (voice->handle && *voice->handle) {
            fn_710120d54c(*voice->handle, 2);
            fn_7101209300(voice->handle);
        }
        if (voice->player) {
            fn_7101206630(voice->player, 2);
            fn_7101206578(voice->player);
        }
        voice->field_0x16f = true;
    }
    fn_7101206444(voice->player, target);
    voice->entries = *reinterpret_cast<const UnkSoundEntries_l0f44cc*>(command);
    voice->playing = true;
    voice->entry = 0;
    voice->field_0x16d = false;
    voice->field_0x16e = false;
    voice->field_0x16f = false;
    voice->field_0x170 = false;
}
