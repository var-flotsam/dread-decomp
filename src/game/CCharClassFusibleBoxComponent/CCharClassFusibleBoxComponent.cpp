// In the CCharClassFusibleBoxComponent unit: hands the actor (+0x30) one of two timeline ids
// (fn_7100d7c828), chosen by `flag`; returns 0. Names and layouts are placeholders.
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern const u64 g_unk_710164ae48;
extern const u64 g_unk_710164ae50;

struct UnkFusibleBox {
    u8 pad_0x0[0x30];
    UnkActor* actor;
};

u32 fn_7100881770(UnkFusibleBox* self, bool flag) {
    fn_7100d7c828(self->actor, flag ? &g_unk_710164ae48 : &g_unk_710164ae50, false);
    return 0;
}
