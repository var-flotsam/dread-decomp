// CBossAIComponent's unit (the name from reflection): the reflected invoker of fn_71003ae428 with a constant
// true and two bool arguments read from the argument values; returns the empty value. Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::refl_ArgValue;
using base::reflection::UnkReflValue;

struct UnkStruct_71003ae428;
void fn_71003ae428(UnkStruct_71003ae428*, bool, bool, bool);

UnkReflValue fn_71003b0e60(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* values = args[0];
    UnkStruct_71003ae428* target = *reinterpret_cast<UnkStruct_71003ae428**>(obj + 8);
    bool a = *refl_ArgValue<u8>(&values[0]);
    bool b = *refl_ArgValue<bool>(&values[1]);
    fn_71003ae428(target, true, a, b);
    return base::reflection::refl_EmptyValue();
}
