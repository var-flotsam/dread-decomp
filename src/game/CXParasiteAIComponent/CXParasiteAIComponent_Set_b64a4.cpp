// CXParasiteAIComponent unit (tools/tu_map.py, the name from reflection): stores a value at +0x14724,
// raises the flag at +0x147b8 and passes the bool on to fn_71006ea3fc. Names are placeholders.
#include "types.h"

struct UnkXParasiteAI_b64a4 {
    u8 pad_0x0[0x14724];
    u32 field_0x14724;
    u8 pad_0x14728[0x90];
    bool field_0x147b8;
};

void fn_71006ea3fc(UnkXParasiteAI_b64a4* self, bool flag);

void fn_71006eba00(UnkXParasiteAI_b64a4* self, u32 value, bool flag) {
    self->field_0x147b8 = true;
    self->field_0x14724 = value;
    fn_71006ea3fc(self, flag);
}
