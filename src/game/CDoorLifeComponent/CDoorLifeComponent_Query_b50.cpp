// CDoorLifeComponent unit (tools/tu_map.py, the name from reflection): a reflected query: stores the
// answer of fn_71008d5c1c as a bool in the value `out` (in place when it already holds a compatible
// type, else by resetting and rebuilding it) and reports success. Names are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU1376.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkDoorLife;

bool fn_71008d5b80(char* self, void*, UnkReflValue* out) {
    bool value = fn_71008d5c1c(reinterpret_cast<UnkDoorLife*>(self));
    const CType* t = fn_710000d228();
    const CType* cur = reinterpret_cast<const CType*>(out->type);
    if (cur && out->field_0x14 == 0 &&
        (cur == t ? *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(t) + 0x48) != nullptr
                  : fn_710009d52c(t, cur))) {
        fn_71000a1cc8(out, t, &value);
    } else {
        fn_71000a1c60(out);
        fn_71000a18d4(out, t, &value, false);
    }
    return true;
}
