// CBreakableTileGroup's unit: the constructor of one 0xa8-byte breakable tile (game/UnkBreakableTile.h): no
// index, an empty name id, three id/path slots set from the unit's three static pairs.
// Names and layouts are placeholders.
#include "game/UnkBreakableTile.h"
#include "types.h"

UnkBreakableTile_b49::UnkBreakableTile_b49()
    : index(-1), field_0x4(0), field_0x8(0), field_0xc(0), field_0x10(0), field_0x14(0), field_0x18(0.5f),
      field_0x1c(0.0f), field_0x28(0.0f), field_0x2c(0.0f),
      field_0x30(nullptr), field_0x38(1), field_0x3c(false), field_0x40(-1.0f), field_0x44(0),
      render_object(0xffffffff), field_0x50(0), field_0x58(0) {
    for (int i = 0; i < 3; i++)
        fn_71000267f8(reinterpret_cast<UnkStrIdPathResource*>(&slots[i]),
                      reinterpret_cast<const UnkStrIdPathPair&>(g_unk_7172795f50[i]));
}
