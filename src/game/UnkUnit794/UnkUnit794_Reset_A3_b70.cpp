// Unit 794 (no hint; its tunable is g_unk_71726e0aa8): when fn_7100500790 holds, hands over to
// fn_7100500a50; otherwise reloads the timer (+0x18) and the delay (+0x1c, zero when the owner's
// vf_0x2f8 holds) from the tunable, clears +0x24 and fades out (0.2, fn_7100105a80) the sound its
// handle (+0x38, checked against the id at +0x40) still points at. Names and layouts are placeholders.
#include "base/snd/UnkSound.h"
#include "game_s2/UnkChargeComponent.h"
#include "types.h"

struct UnkOwner_A3_b70 {
    void** vtable;
    bool vf_0x2f8() { return reinterpret_cast<bool (*)(UnkOwner_A3_b70*)>(vtable[0x2f8 / 8])(this); }
};

struct UnkSoundHandle_A3_b70 {
    UnkSound* sound;
    u64 id;
};

struct UnkUnit794_A3_b70 {
    UnkOwner_A3_b70* owner;
    u8 pad_0x8[0x10];
    f32 field_0x18;
    f32 field_0x1c;
    u8 pad_0x20[4];
    u8 field_0x24;
    u8 pad_0x25[0x13];
    UnkSoundHandle_A3_b70* handle;
    u64 handle_id;
};

bool fn_7100500790(UnkUnit794_A3_b70* self);
void fn_7100500a50(UnkUnit794_A3_b70* self, f32 value);

static inline f32 TunableAt_A3_b70(u32 offset) {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_71726e0aa8) + offset);
}

void fn_71005006c0(UnkUnit794_A3_b70* self, f32 value) {
    if (fn_7100500790(self)) {
        fn_7100500a50(self, value);
        return;
    }
    self->field_0x18 = TunableAt_A3_b70(0x84);
    if (self->owner->vf_0x2f8())
        self->field_0x1c = 0.0f;
    else
        self->field_0x1c = TunableAt_A3_b70(0x88);
    self->field_0x24 = 0;
    UnkSoundHandle_A3_b70* handle = self->handle;
    if (handle && handle->id == self->handle_id && self->handle_id != ~0ull && handle->sound)
        fn_7100105a80(handle->sound, 0.2f, 0.0f);
}
