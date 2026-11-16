// CBreakableTileGroup's unit: sets the group's origin and places every tile (100 units a cell).
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CBreakableTileGroup.h"
#include "types.h"

struct UnkTileGroup;

// One tile of a group.
struct UnkQueryObj {
    u32 field_0x0;  // index
    u32 field_0x4;  // type
    u32 field_0x8;  // cell x
    u32 field_0xc;  // cell y
    u8 pad_0x10[0x18];
    f32 field_0x28;  // x
    f32 field_0x2c;  // y
    UnkTileGroup* field_0x30;
    s32 state;  // 0x38
    bool field_0x3c;
    u8 pad_0x3d[0xb];
    u64 field_0x48;
    u8 pad_0x50[0x58];
};
static_assert(sizeof(UnkQueryObj) == 0xa8, "UnkQueryObj placeholder layout");

struct UnkTileCollider;

struct UnkTileGroup {
    u8 pad_0x0[0x14];
    bool field_0x14;
    u8 pad_0x15[0x3];
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    s32 field_0x24;
    s32 field_0x28;
    u8 pad_0x2c[0x14];
    u32 field_0x40;
    UnkQueryObj* field_0x48;
    u8 pad_0x50[0x8];
    u32* field_0x58;
    u8 pad_0x60[0x68];
    u32 field_0xc8;
    UnkTileCollider* field_0xd0;
};

using base::math::CVector3D;

void fn_7100c9e968(UnkQueryObj* tile);

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100ca1da4(UnkTileGroup* group, const CVector3D* origin) {
    group->field_0x18 = origin->x;
    group->field_0x1c = origin->y;
    group->field_0x20 = origin->z;
    for (u32 i = 0; i < group->field_0x40; i++) {
        UnkQueryObj* tile = &group->field_0x48[i];
        tile->field_0x28 = group->field_0x18 + (f32)tile->field_0x8 * 100.0f;
        tile->field_0x2c = group->field_0x1c + (f32)tile->field_0xc * 100.0f;
        fn_7100c9e968(tile);
    }
    fn_7100ca1180(group);
}
