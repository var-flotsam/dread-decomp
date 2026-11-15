// Unit 2702 (hint CAIComponent): when a ratio on the component leaves 0..1, runs the component's
// slot 0x668 unless the owner's frozen component says otherwise or (unless forced) the owner's
// target has no active component. Names and layouts are placeholders.
#include "game/CFrozenComponent.h"
#include "types.h"

struct UnkActiveComponent_l100498 {
    void** vtable;
    bool vf_0xe0() { return reinterpret_cast<bool (*)(UnkActiveComponent_l100498*)>(vtable[0xe0 / 8])(this); }
};

struct UnkTarget_l100498 {
    u8 pad_0x0[0x1a0];
    UnkActiveComponent_l100498* field_0x1a0;
};

struct UnkOwner_l100498 {
    u8 pad_0x0[0x278];
    UnkTarget_l100498* field_0x278;
    u8 pad_0x280[0x48];
    void* field_0x2c8;
};

struct UnkSelf_l100498 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkOwner_l100498* owner;  // 0x30
    u8 pad_0x38[0x144e4];
    f32 ratio;  // 0x1451c

    void vf_0x668() { reinterpret_cast<void (*)(UnkSelf_l100498*)>(vtable[0x668 / 8])(this); }
};

static inline bool TargetActive(UnkSelf_l100498* self) {
    UnkTarget_l100498* target = self->owner->field_0x278;
    if (!target)
        return false;
    UnkActiveComponent_l100498* active = target->field_0x1a0;
    return active && active->vf_0xe0();
}

void fn_7101100498(UnkSelf_l100498* self, bool forced) {
    f32 ratio = self->ratio;
    if (!(ratio <= 1.0f) || ratio < 0.0f) {
        void* frozen = self->owner->field_0x2c8;
        bool ok;
        if (frozen && fn_71007f9f20(frozen))
            ok = false;
        else
            ok = forced || TargetActive(self);
        if (ok)
            self->vf_0x668();
    }
}
