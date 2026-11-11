// CDoorLifeComponent's unit: the reflected invoker of the open/close setter (as fn_71008d51e4,
// CDoorLifeComponent_Set_A5_b65.cpp): two bool arguments, the empty status {0, 0, 2, 1} as in
// CEmmyValveComponent_ReflInvoker_A5_b65.cpp. Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU1376.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkDoorLife_b44a2;

static inline __attribute__((always_inline)) void* ArgPtr_A5_b66(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71008dc890(void*, char* obj, UnkReflValue* const* args) {
    UnkDoorLife_b44a2* target = *reinterpret_cast<UnkDoorLife_b44a2**>(obj + 8);
    UnkReflValue* values = args[0];
    bool open = *static_cast<bool*>(ArgPtr_A5_b66(&values[0]));
    u8 quiet = *static_cast<u8*>(ArgPtr_A5_b66(&values[1]));  // read as a byte, not a bool
    if (open)
        fn_71008d0ef4(target);
    else
        fn_71008d498c(target, !quiet, 1, 1);
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
