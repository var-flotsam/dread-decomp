// Functions in the CCubeMapComponent unit (tools/tu_map.py). Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;
using base::reflection::refl_ArgValue;
using base::reflection::refl_EmptyValue;

struct UnkCubeMapArg;
void fn_71001f3964(void* target, const UnkCubeMapArg* value);

struct UnkCubeMapTarget {
    u8 pad_0x0[0x50];
    char* field_0x50;
    u8 pad_0x58[0x8];
    char* field_0x60;
};

// A property setter: passes the argument (null when it has no type) to the members at +0x18 of
// both objects at +0x50 and +0x60.
UnkReflValue fn_71007a0804(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    UnkCubeMapTarget* target = *reinterpret_cast<UnkCubeMapTarget**>(obj + 8);
    const UnkCubeMapArg* value = arg->type ? refl_ArgValue<UnkCubeMapArg>(arg) : nullptr;
    fn_71001f3964(target->field_0x50 + 0x18, value);
    fn_71001f3964(target->field_0x60 + 0x18, value);
    return refl_EmptyValue();
}

void fn_71001f3e88(void* target, bool value);

// A property setter: passes the flag argument to the members at +0x18 of both objects at +0x50 and
// +0x60.
UnkReflValue fn_71007a0c2c(void*, char* obj, UnkReflValue* const* args) {
    UnkCubeMapTarget* target = *reinterpret_cast<UnkCubeMapTarget**>(obj + 8);
    bool value = *refl_ArgValue<bool>(args[0]);
    fn_71001f3e88(target->field_0x50 + 0x18, value);
    fn_71001f3e88(target->field_0x60 + 0x18, value);
    return refl_EmptyValue();
}
