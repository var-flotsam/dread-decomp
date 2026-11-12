// CPositionalSoundComponent unit: resets whichever of the two sound handles refers to the given object.
// Names and layouts are placeholders.
#include "game/UnkTU101.h"

struct UnkSoundSlot {
    void* object;
    u64 generation;
};

struct UnkSoundHandle {
    UnkSoundSlot* slot;
    u64 generation;
};

struct UnkPositionalSoundBase {
    u8 pad_0x0[0x88];
    UnkSoundHandle handle_a;
    UnkSoundHandle handle_b;
};

static inline bool HandleRefers(const UnkSoundHandle& h, void* object) {
    UnkSoundSlot* slot = h.slot;
    if (slot != nullptr && slot->generation == h.generation && h.generation != (u64)-1) {
        void* held = slot->object;
        if (held != nullptr)
            return held == object;
    }
    return false;
}

void fn_710114ccd0(u8* self, void* object) {
    if (object == nullptr)
        return;
    UnkPositionalSoundBase* base = (UnkPositionalSoundBase*)(self - 0x40);
    // keeps the owner-base address computed once, as the original does
    asm("" : "+r"(base));
    UnkSoundHandle* hit;
    if (HandleRefers(base->handle_a, object))
        hit = &base->handle_a;
    else if (HandleRefers(base->handle_b, object))
        hit = &base->handle_b;
    else
        return;
    *hit = g_unk_7108b6c460;
}
