// CCharClassSoundEvent's unit (tools/tu_map.py unit 2297): the event plays its sound (fn_7100cdc2f0) with
// its parameters and the given target, unless the GUI (g_unk_7101d47330) answers fn_7101198688. Names
// and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU2730.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101d47330;

struct UnkCharClassSoundEvent_b0080 {
    u8 pad_0x0[0x10];
    u8 field_0x10[8];
    u8 field_0x18[8];
    u8 field_0x20[8];
    u8 field_0x28[8];
    u8 field_0x30[8];
    u8 field_0x38[8];
    u8 field_0x40[8];
    u8 field_0x48[8];
    u8 pad_0x50[8];
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    u8 field_0x6c;
    f32 field_0x70;
    f32 field_0x74;
    f32 field_0x78;
    f32 field_0x7c;
    u8 field_0x80;
    u8 field_0x81;
    u8 pad_0x82[6];
    u8 field_0x88[8];
    u8 field_0x90[8];
    u32 field_0x98;
};
static_assert(__builtin_offsetof(UnkCharClassSoundEvent_b0080, field_0x98) == 0x98, "layout");

void fn_7100cdc2f0(UnkCharClassSoundEvent_b0080* self, const void* a10, const void* a18, const void* a20,
                   const void* a28, const void* a30, const void* a38, const void* a40, f32 f58, f32 f5c,
                   f32 f60, f32 f64, f32 f68, f32 f70, f32 f74, f32 f7c, const void* a48, const void* a90,
                   const CStrId* id, const void* a88, u8 b6c, u32 u98, u8 b80, u8 b81, f32 f78, void* arg);

void fn_7100cdebcc(UnkCharClassSoundEvent_b0080* self, void* arg) {
    if (fn_7101198688(g_unk_7101d47330))
        return;
    fn_7100cdc2f0(self, self->field_0x10, self->field_0x18, self->field_0x20, self->field_0x28, self->field_0x30,
                  self->field_0x38, self->field_0x40, self->field_0x58, self->field_0x5c, self->field_0x60,
                  self->field_0x64, self->field_0x68, self->field_0x70, self->field_0x74, self->field_0x7c,
                  self->field_0x48, self->field_0x90, &g_unk_7101d470e0, self->field_0x88, self->field_0x6c,
                  self->field_0x98, self->field_0x80, self->field_0x81, self->field_0x78, arg);
}
