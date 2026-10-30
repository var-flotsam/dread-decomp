// The neighbouring unit's switch (unit 133 of tools/tu_map.py; fn_7100146e60 next door is unit
// 134's): UnkSwitchSet in game/UnkSwitchObject.h, with unit 116's copies.
#include "base/UnkCloneCalls.h"
#include "game/UnkSwitchObject.h"
#include "types.h"

void fn_7100208004(void* obj, bool flag);

void fn_71001f7388(void* obj, bool flag);

void fn_7100145e94(UnkSwitchObject* self, bool enable) { UnkSwitchSet<fn_7100208004>(self, enable); }
void fn_71001464f0(UnkSwitchObject* self, bool enable) { UnkSwitchSet<fn_7100208004>(self, enable); }
// Unit 116's two copies, on a layout whose later members sit 0x58 bytes further on. They keep
// UnkSwitchObject's member names, which the template uses.
struct UnkSwitchObject116 {
    u8 pad_0x0[0x28];
    u8 field_0x28[0xf9];
    bool field_0x121;
    u8 pad_0x122[0x9e];
    char field_0x168[0xd8];  // at 0x1c0
    bool field_0x240;        // at 0x298
    u8 pad_0x299[3];
    s32 field_0x244;  // at 0x29c
};
static_assert(__builtin_offsetof(UnkSwitchObject116, field_0x244) == 0x29c, "UnkSwitchObject116 layout");

void fn_7100140e4c(UnkSwitchObject116* self, bool enable) { UnkSwitchSet<fn_71001f7388>(self, enable); }
void fn_71001414ac(UnkSwitchObject116* self, bool enable) { UnkSwitchSet<fn_71001f7388>(self, enable); }
