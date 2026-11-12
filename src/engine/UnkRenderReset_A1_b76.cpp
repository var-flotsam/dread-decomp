// Unit 260 of tools/tu_map.py (render setup, no class hint): two resets of the object at +0xf0 that
// hand it a global string id, the blocks at +0x118 and +0x308, then clear it through a fixed run of
// its methods; fn_71001db968 and fn_71001dbfb0 differ only in the third call (fn_71001cea20 or
// fn_71001ced1c). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU259.h"
#include "types.h"

using base::global::CStrId;

struct UnkRenderTargetA1;

// A global id the resets pass on (a hidden unit global).
extern CStrId g_unk_716e596168 __attribute__((visibility("hidden")));

void fn_71001cea20(UnkRenderTargetA1* target);
void fn_71001ced1c(UnkRenderTargetA1* target);

struct UnkRenderOwnerA1 {
    u8 pad_0x0[0xf0];
    UnkRenderTargetA1* target;  // 0xf0
    u8 pad_0xf8[0x20];
    u8 block_0x118[0x1f0];  // 0x118
    u8 block_0x308[0x8];    // 0x308
};

template <void (*Third)(UnkRenderTargetA1*)>
static inline void ResetA1(UnkRenderOwnerA1* self) {
    fn_71001cdd6c(self->target, g_unk_716e596168);
    fn_71001cdd74(self->target, self->block_0x118);
    fn_71001cdba4(self->target, self->block_0x308);
    Third(self->target);
    fn_71001cdd18(self->target);
    fn_71001cde48(self->target);
    fn_71001cb604(self->target, 0, nullptr, nullptr, nullptr, nullptr);
    fn_71001cdd70(self->target);
}

void fn_71001db968(UnkRenderOwnerA1* self) {
    ResetA1<fn_71001cea20>(self);
}

void fn_71001dbfb0(UnkRenderOwnerA1* self) {
    ResetA1<fn_71001ced1c>(self);
}
