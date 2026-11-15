// CBTObserverComponent unit: a float tunable of the breakable tile manager (g_unk_7172796920) picked by
// a type 1 to 9, or -1 for a type outside that. Names and layouts are placeholders.
#include "game/CBreakableTileManager/CTunableBreakableTileManager.h"

struct UnkBreakableTunables : UnkTunable {
    u8 pad_0x8[0x1c];
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
};

f32 fn_7100ca6af8(u32 type) {
    const UnkBreakableTunables& tunables = static_cast<const UnkBreakableTunables&>(g_unk_7172796920);
    switch (type) {
    case 1:
        return tunables.field_0x24;
    case 2:
        return tunables.field_0x28;
    case 3:
        return tunables.field_0x2c;
    case 4:
        return tunables.field_0x30;
    case 5:
        return tunables.field_0x34;
    case 6:
        return tunables.field_0x38;
    case 7:
        return tunables.field_0x3c;
    case 9:
        return tunables.field_0x40;
    default:
        return -1.0f;
    }
}
