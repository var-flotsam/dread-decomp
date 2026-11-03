// GUI::CDisplayObject unit (tools/tu_map.py, the name from reflection): a reflected function that
// passes its float and bool arguments to the virtual at 0x70 of the object at 8 and returns the empty
// value. Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

UnkReflValue fn_71011d36a4(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    void** vtable = *reinterpret_cast<void***>(target);
    auto fn = reinterpret_cast<void (*)(char*, f32, bool)>(vtable[0x70 / 8]);
    UnkReflValue* a = args[0];
    fn(target, *base::reflection::refl_ArgValue<f32>(&a[0]), *base::reflection::refl_ArgValue<bool>(&a[1]));
    return base::reflection::refl_EmptyValue();
}
