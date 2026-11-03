// Unit 99 (base::snd::CSoundSystemATK): advancing a voice to its next entry. While entries are
// left the current sound is stopped (only when `stop`, fade 2) and the next one started with the
// stored start parameters (fn_71000f4ebc); past the last, the sound and the player are stopped,
// the entry reset to -1 and two flags (+0x16f, +0x170) set. The layout is UnkSoundStart's
// (src/base/snd/CSoundSystemATK.cpp); names are placeholders.

#include "types.h"

struct UnkSoundStart;
bool fn_71000f4ebc(UnkSoundStart* self, u32 id, u32 c, f32 a, f32 b);
// nn::atk (library): stopping a sound and a player, with a fade of `frames`.
void fn_710120d54c(void* sound, u32 frames);
void fn_7101206630(u8* player, u32 frames);

struct UnkSoundVoice_71000f4a68 {
    u8 pad_0x0[0x28];
    void** handle;
    u8* player;
    s32 entry_count;
    u8 entries[0x100];
    s32 entry;
    u8 pad_0x140[0x1c];
    u32 start_id;
    f32 start_a;
    f32 start_b;
    u32 start_c;
    u8 pad_0x16c[0x3];
    bool field_0x16f;
    bool field_0x170;
};
static_assert(__builtin_offsetof(UnkSoundVoice_71000f4a68, entry) == 0x13c, "");
static_assert(__builtin_offsetof(UnkSoundVoice_71000f4a68, start_id) == 0x15c, "");

bool fn_71000f4a68(UnkSoundVoice_71000f4a68* self, bool stop) {
    s32 next = ++self->entry;
    if (next < self->entry_count) {
        if (self->handle && stop && *self->handle)
            fn_710120d54c(*self->handle, 2);
        return fn_71000f4ebc(reinterpret_cast<UnkSoundStart*>(self), self->start_id, self->start_c,
                             self->start_a, self->start_b);
    }
    if (self->handle && *self->handle)
        fn_710120d54c(*self->handle, 2);
    if (self->player)
        fn_7101206630(self->player, 2);
    self->entry = -1;
    self->field_0x16f = true;
    self->field_0x170 = true;
    return false;
}
