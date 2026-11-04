// CAimComponent's unit (tools/tu_map.py unit 437): a reflected invoker (src/reflection/CAimComponent.cpp)
// that adds the string id argument (null without a type) to, or removes it from, the key list at +0x6e8
// by the flag argument, as fn_71003448d4 (src/game/CAimComponent/UnkAimKeyListToggle_b43.cpp) does.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;
using base::reflection::refl_ArgValue;
using base::reflection::refl_EmptyValue;

UnkReflValue fn_7100354274(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    UnkReflValue* arg = args[0];
    bool on = *refl_ArgValue<bool>(&arg[0]);
    CStrId* key = arg[1].type ? const_cast<CStrId*>(refl_ArgValue<CStrId>(&arg[1])) : nullptr;
    UnkGuiKeyList* list = reinterpret_cast<UnkGuiKeyList*>(target + 0x6e8);
    if (on)
        fn_710002fd10(list, key);
    else
        fn_710002ffb4(list, key);
    return refl_EmptyValue();
}
