// CCutsceneComponent's unit: a reflected function's invoker with two u32 arguments: looks up the entry
// the first names (fn_710106b284 on the object at +0x200 of the target) and stores the second in it
// (+8); answers the empty status {0, 0, 2, 1}. Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkCutsceneEntry_A5_b65 {
    u64 field_0x0;
    u32 field_0x8;
};

struct UnkCutsceneTarget2_A5_b65 {
    u8 pad_0x0[0x200];
    void* field_0x200;
};

UnkCutsceneEntry_A5_b65* fn_710106b284(void* object, u32 id);

static inline __attribute__((always_inline)) void* ArgPtr2_A5_b65(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71007ade14(void*, char* obj, UnkReflValue* const* args) {
    UnkCutsceneTarget2_A5_b65* target = *reinterpret_cast<UnkCutsceneTarget2_A5_b65**>(obj + 8);
    UnkReflValue* values = args[0];
    u32 id = *static_cast<u32*>(ArgPtr2_A5_b65(&values[0]));
    u32 value = *static_cast<u32*>(ArgPtr2_A5_b65(&values[1]));
    fn_710106b284(target->field_0x200, id)->field_0x8 = value;
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
