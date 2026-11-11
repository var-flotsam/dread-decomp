// CPowerUpLifeComponent unit (tools/tu_map.py, the name from reflection): reads a string from the
// stream ("s") and moves its string id into +0x3d0; always succeeds. Names are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::UnkStrId;

bool fn_71008e4f30(char* self, void* stream) {
    char text[0x80];
    fn_710007cf30(stream, "s", text);
    *reinterpret_cast<UnkStrId*>(self + 0x3d0) = UnkStrId(text, true);
    return true;
}
