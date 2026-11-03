// CMorphBallMovement unit (tools/tu_map.py, the name from reflection): a reflected setter that reads a
// float from its argument, keeps it within [0.1, 1] and stores it at +0x5c of the object at 8; it returns
// an empty reflected value. Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

UnkReflValue fn_710094ffe4(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    const f32* in;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        in = reinterpret_cast<const f32*>(arg->field_0x0);
    else
        in = reinterpret_cast<const f32*>(arg);
    f32 v = *in;
    f32 lo = 1.0f < v ? 1.0f : v;
    f32 out = 0.1f;
    if (!(v <= 0.1f))
        out = lo;
    *reinterpret_cast<f32*>(target + 0x5c) = out;
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
