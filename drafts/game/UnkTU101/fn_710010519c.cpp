// Unit 101: resets an effect record's three float triples to their stored defaults and attaches an owner.
// Names and layouts are placeholders.
#include "base/snd/UnkSoundVoice2.h"
#include "game/UnkTU101.h"
#include "types.h"

extern s32 g_unk_7108b6c470 __attribute__((visibility("hidden")));

void fn_710010519c(UnkSoundVoice2_b56* self, void* owner, u32 value, bool flag) {
    g_unk_7108b6c470++;
    self->flag = flag;
    UnkSoundTriple a(self->def_a);
    UnkSoundTriple b(self->def_b);
    UnkSoundTriple c(self->def_c);
    self->owner = owner;
    self->owner_next = nullptr;
    self->a = a;
    self->one_z = 1.0f;
    self->id = value;
    self->b = b;
    self->c = c;
    self->one_x = 1.0f;
    self->one_y = 1.0f;
}
