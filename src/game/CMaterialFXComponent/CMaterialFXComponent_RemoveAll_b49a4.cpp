// CMaterialFXComponent unit (tools/tu_map.py): calls fn_7100926494 for every index of the list at
// +0x48, last to first. Names are placeholders.
#include "types.h"

struct UnkMaterialFX_b49a4 {
    u8 pad_0x0[0x48];
    u32 count;
};

void fn_7100926494(UnkMaterialFX_b49a4* self, s32 index);

void fn_7100927194(UnkMaterialFX_b49a4* self) {
    for (s32 i = (s32)(self->count - 1); i >= 0; --i)
        fn_7100926494(self, i);
}
