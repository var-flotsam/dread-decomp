// GUI::CLabel unit (tools/tu_map.py, the name from reflection): reflected setters of the text
// (from the argument through fn_7100066c00, null when it has no type) and of the string id
// at +0x260 of the label at 8; when it changes, it clears the word at +0x298 and runs the virtual at
// 0x88. Returns the empty value. Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGuiLabel.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkLabel_b40a3 {
    void** vtable;
    u8 pad_0x8[0x258];
    CStrId text;  // 0x260
    u8 pad_0x268[0x30];
    u64 field_0x298;
};

UnkReflValue fn_71011efb58(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    auto* target = *reinterpret_cast<UnkLabel_b40a3**>(obj + 8);
    const CStrId& value = *base::reflection::refl_ArgValue<CStrId>(arg);
    if (value.data != target->text.data) {
        target->text = value;
        target->field_0x298 = 0;
        reinterpret_cast<void (*)(UnkLabel_b40a3*)>(target->vtable[0x88 / 8])(target);
    }
    return base::reflection::refl_EmptyValue();
}

void fn_7100066c00(void* self, const void* source);
void fn_7100066e44(void*);

static inline __attribute__((always_inline)) void* ArgPtr_b40a3(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71011efde0(void*, char* obj, UnkReflValue* const* args) {
    auto* target = *reinterpret_cast<UnkGuiDisplayObject**>(obj + 8);
    UnkGuiText text;
    fn_7100066c00(&text, ArgPtr_b40a3(args[0]));
    fn_71011ec468(target, text, false);
    fn_7100066e44(&text);
    return base::reflection::refl_EmptyValue();
}
