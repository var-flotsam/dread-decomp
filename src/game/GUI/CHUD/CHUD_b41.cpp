// GUI::CHUD unit (tools/tu_map.py, the name from reflection): a reflected function that takes an
// object (a pointer argument, null when it has no type) and looks its id (+0x30) up in the game
// manager (fn_7100debbc8, with the kind g_unk_71727bebb8); the result is unused. Returns an empty
// value. Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkWeakTarget;

UnkReflValue fn_71011e68e8(void*, char*, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* const* object;
    if (!a->type)
        object = nullptr;
    else if (a->field_0x14 == 1 || *reinterpret_cast<const u64*>(a->type + 0x18) > 8)
        object = static_cast<char* const*>(a->field_0x0);
    else
        object = reinterpret_cast<char* const*>(a);
    UnkGameManager* manager = g_unk_7101d47310;
    u32 kind_value = g_unk_71727bebb8;
    u64 key = *reinterpret_cast<const u64*>(*object + 0x30);
    alignas(8) u32 kind = kind_value;
    fn_7100debbc8(manager, &key, &kind);
    return base::reflection::refl_EmptyValue();
}
