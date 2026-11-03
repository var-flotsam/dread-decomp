// In the CEmmyAIComponent unit: a reflected setter of the flag at +0x14c14 of the object (when the
// value changes it stores it and runs the update fn_7100540dd0; it returns the empty value), and a
// setter of the flag at +0x27720 that runs a virtual on change. Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_7100540dd0(char* self);

UnkReflValue fn_71005658dc(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    bool value = *base::reflection::refl_ArgValue<bool>(arg);
    bool* flag = reinterpret_cast<bool*>(target + 0x14c14);
    if (*flag != value) {
        *flag = value;
        fn_7100540dd0(target);
    }
    return base::reflection::refl_EmptyValue();
}

// Sets the flag at +0x27720 of the component; when it changes, runs the virtual at slot 0x858.
void fn_7100541e34(char* self, bool value) {
    bool* flag = reinterpret_cast<bool*>(self + 0x27720);
    if (*flag != value) {
        *flag = value;
        void** vtable = *reinterpret_cast<void***>(self);
        reinterpret_cast<void (*)(char*)>(vtable[0x858 / 8])(self);
    }
}
