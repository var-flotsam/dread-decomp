// CXParasiteAIComponent unit (tools/tu_map.py, the name from reflection): stores a value at +0x14724,
// raises the flag at +0x147b8 and passes the bool on to fn_71006ea3fc. Names are placeholders.
#include "game/UnkTU1102.h"
#include "game/UnkXParasiteAI.h"
#include "types.h"

void fn_71006eba00(UnkXParasiteAI* self, u32 value, bool flag) {
    self->field_0x147b8 = true;
    self->field_0x14724 = value;
    fn_71006ea3fc(self, flag);
}
