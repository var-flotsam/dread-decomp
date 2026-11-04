// CAbilityEnergyWave's unit (the name from reflection): the reflected invoker of fn_71002fda50 (two bool
// arguments, read from the argument values; the first parameter is unused); returns the empty value.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::refl_ArgValue;
using base::reflection::UnkReflValue;

struct UnkStruct_71002fda50;
void fn_71002fda50(UnkStruct_71002fda50*, bool, bool);

UnkReflValue fn_7100302eb0(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* values = args[0];
    UnkStruct_71002fda50* target = *reinterpret_cast<UnkStruct_71002fda50**>(obj + 8);
    bool all = *refl_ArgValue<u8>(&values[0]);
    bool force_off = *refl_ArgValue<bool>(&values[1]);
    fn_71002fda50(target, all, force_off);
    return base::reflection::refl_EmptyValue();
}
