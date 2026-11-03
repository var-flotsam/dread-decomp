// CDemolitionBlockLifeComponent unit (tools/tu_map.py): returns 1 when the third argument's flag
// 0x1000 at +0xb4 is set and the fourth argument's chain (+0, +0x10, +0x10) is non-null; otherwise
// falls back to fn_7101160894 (the base answer). Names are placeholders.
#include "types.h"

u32 fn_7101160894();

struct UnkDemoLink2_b49a4 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};
struct UnkDemoLink1_b49a4 {
    u8 pad_0x0[0x10];
    UnkDemoLink2_b49a4* field_0x10;
};
struct UnkDemoLink0_b49a4 {
    UnkDemoLink1_b49a4* field_0x0;
};
struct UnkDemoInfo_b49a4 {
    u8 pad_0x0[0xb4];
    u32 flags;
};

u32 fn_71008c8694(void* self, void* a1, UnkDemoInfo_b49a4* info, UnkDemoLink0_b49a4* link) {
    if (UnkDemoLink1_b49a4* l1 = link->field_0x0) {
        UnkDemoLink2_b49a4* l2 = l1->field_0x10;
        u32 bit = info->flags & 0x1000;
        if (l2 != nullptr && bit != 0 && l2->field_0x10 != nullptr)
            return 1;
    }
    return fn_7101160894();
}
