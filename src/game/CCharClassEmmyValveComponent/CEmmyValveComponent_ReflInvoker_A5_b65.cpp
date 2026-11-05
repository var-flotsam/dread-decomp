// CCharClassEmmyValveComponent's unit (CEmmyValveComponent): a reflected function's invoker that passes
// its two bool arguments to fn_71007b3db8 (CEmmyValveComponent_Request_A5_b64.cpp) and answers the empty
// status {0, 0, 2, 1}, as fn_71002609ac (src/base/reflection/UnkUnit346/UnkUnit346_ReflSetFlag_b60.cpp).
// Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkEmmyValve_A5_b64;
void fn_71007b3db8(UnkEmmyValve_A5_b64* self, bool open, bool set);

static inline __attribute__((always_inline)) void* ArgPtr_A5_b65(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71007b4bb0(void*, char* obj, UnkReflValue* const* args) {
    UnkEmmyValve_A5_b64* target = *reinterpret_cast<UnkEmmyValve_A5_b64**>(obj + 8);
    UnkReflValue* values = args[0];
    bool open = *static_cast<bool*>(ArgPtr_A5_b65(&values[0]));
    bool set = *static_cast<bool*>(ArgPtr_A5_b65(&values[1]));
    fn_71007b3db8(target, open, set);
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
