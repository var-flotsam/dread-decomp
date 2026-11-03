// CCutsceneComponent unit (tools/tu_map.py unit 1208). fn_71007a9e30: with no player (+0x200) and the
// byte at +0x20c clear, calls fn_71007a6758 unless the id at +0x148 is the empty id. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkCutsceneComponent_b49a3 {
    u8 pad_0x0[0x148];
    base::global::CStrId id;  // 0x148
    u8 pad_0x150[0xb0];
    void* player;  // 0x200
    u8 pad_0x208[0x4];
    u8 field_0x20c;
};

void fn_71007a6758(UnkCutsceneComponent_b49a3* self);

void fn_71007a9e30(UnkCutsceneComponent_b49a3* self) {
    if (self->player != nullptr || self->field_0x20c != 0)
        return;
    if (self->id.data == g_unk_7101d470e0.data)
        return;
    fn_71007a6758(self);
}
