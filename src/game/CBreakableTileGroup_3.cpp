// A function in the CBreakableTileGroup unit: fn_7100c9fe00 (re)starts the tile's timer at +0x40 from the
// table at 0x71016478e8 when it is not running (or when forced). A timer of zero breaks it at once:
// the delay at +0x14 (when set) is reset from the game manager's +0x2444, the timer becomes -1, the state
// moves to 2 (fn_7100c9e6ac), and for mode 1 the delay at +0x10 (when set) is reset from the tunable at
// 0x7172796920 (its +0x48). Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

typedef u32 __attribute__((may_alias)) a_u32;

struct UnkQueryObj;
struct UnkTunable;
void fn_7100c9e6ac(UnkQueryObj* obj, u32 state, bool a, bool b);
extern UnkTunable g_unk_7172796920;
extern const f32 g_unk_71016478e8[] __attribute__((visibility("hidden")));

struct UnkBreakableTile {
    u8 pad_0x0[0x10];
    f32 field_0x10;
    f32 field_0x14;
    u8 pad_0x18[0x28];
    f32 timer;  // 0x40
};

void fn_7100c9fe00(UnkBreakableTile* self, s32 kind, s32 mode, bool flag, bool force) {
    // Copied as a word (the table entry is only read through its bits here).
    if (self->timer < 0.0f || force)
        *reinterpret_cast<a_u32*>(&self->timer) = *reinterpret_cast<const a_u32*>(&g_unk_71016478e8[kind]);
    if (!(self->timer <= 0.0f))
        return;
    if (self->field_0x14 > 0.0f)
        self->field_0x14 = *reinterpret_cast<f32*>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x2444);
    self->timer = -1.0f;
    fn_7100c9e6ac(reinterpret_cast<UnkQueryObj*>(self), 2, false, flag);
    if (mode == 1 && self->field_0x10 > 0.0f)
        self->field_0x10 = *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_7172796920) + 0x48);
}
