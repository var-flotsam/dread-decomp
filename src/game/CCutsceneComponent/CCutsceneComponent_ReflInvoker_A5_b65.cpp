// CCutsceneComponent's unit: a reflected function's invoker (set up in src/reflection/function_inits_01.cpp)
// that passes its argument to fn_7101068f7c on the object at +0x200 of the target (when there is one)
// and answers the empty status {0, 0, 2, 1}, as fn_71002609ac
// (src/base/reflection/UnkUnit346/UnkUnit346_ReflSetFlag_b60.cpp). Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkCutsceneTarget_A5_b65 {
    u8 pad_0x0[0x200];
    void* field_0x200;
};

void fn_7101068f7c(void* object, void* arg);

static inline __attribute__((always_inline)) void* ArgPtr_A5_b65(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71007ad7a0(void*, char* obj, UnkReflValue* const* args) {
    UnkCutsceneTarget_A5_b65* target = *reinterpret_cast<UnkCutsceneTarget_A5_b65**>(obj + 8);
    void* arg = ArgPtr_A5_b65(args[0]);
    if (target->field_0x200)
        fn_7101068f7c(target->field_0x200, arg);
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
