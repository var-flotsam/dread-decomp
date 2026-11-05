// CBreakableVignetteComponent's unit: breaks the vignette once (+0x39): hides its object through the
// render manager until it no longer answers 2 (as fn_7100763318), runs fn_7100762f64, then passes
// the word at +0x18 to the list at +0x60 (fn_710076371c). Names and layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "types.h"

void fn_7100219c6c(UnkRenderManager*, bool, u64, u32, bool);

struct UnkKeyed_A5_b66 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkBreakableVignette_A5_b66 {
    u8 pad_0x0[0x18];
    u64 field_0x18;
    u8 pad_0x20[0x19];
    bool field_0x39;
    u8 pad_0x3a[6];
    UnkKeyed_A5_b66* field_0x40;
    u8 pad_0x48[0x18];
    u8 field_0x60[1];
};

void fn_7100762f64(UnkBreakableVignette_A5_b66* self);
void fn_710076371c(void* list, const u64* value);

void fn_710076351c(UnkBreakableVignette_A5_b66* self) {
    if (self->field_0x39)
        return;
    self->field_0x39 = true;
    auto show = reinterpret_cast<u32 (*)(UnkRenderManager*, bool, u64, u32, bool)>(fn_7100219c6c);
    while (show(g_unk_7101d47300, !self->field_0x39, self->field_0x40->key, 0, false) == 2) {
    }
    fn_7100762f64(self);
    u64 value = self->field_0x18;
    fn_710076371c(self->field_0x60, &value);
}
