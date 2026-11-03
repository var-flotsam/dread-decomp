// GUI::CText unit (tools/tu_map.py, the name from reflection): a reflected function of the text at 8
// (registered by fn_7100b863ec) that takes an int, formats it as decimal UTF-32 text and passes that
// text to fn_7100b868b0; returns an empty value. Names and layouts are placeholders.
#include <wchar.h>

#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_7100066a50(void* text, const char32_t* value);
void fn_7100066e44(void* text);

struct UnkNumberText_b41a3 {
    u8 unk_0x0[0x20];
};

void fn_7100b868b0(char* self, UnkNumberText_b41a3* text);

UnkReflValue fn_7100b874cc(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    s32 value = *base::reflection::refl_ArgValue<s32>(a);
    {
        wchar_t buf[16];
        swprintf(buf, 16, L"%d", value);
        UnkNumberText_b41a3 text;
        fn_7100066a50(&text, reinterpret_cast<const char32_t*>(buf));
        fn_7100b868b0(target, &text);
        fn_7100066e44(&text);
    }
    return base::reflection::refl_EmptyValue();
}
