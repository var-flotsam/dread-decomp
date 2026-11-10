// Unit 39 of tools/tu_map.py (no reliable name): the identifier spelling of a string, appended to a string value:
// spaces become '_' and the characters ( ) - [ ] _ become the words __PARI__, __PARD__, __DASH__, __CORI__,
// __CORD__ and __UNDS__ (fn_710007f564 appends a string, fn_710007f5ac a character). Names are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

struct UnkStrValue;

void fn_710007f564(void* self, const char* str);  // appends a string

// Appends `str` to the value, spelling out the characters that cannot appear in an identifier.
void fn_7100065b14(const char* str, UnkStrValue& value) {
    CRntString* out = reinterpret_cast<CRntString*>(&value);
    u32 length = __builtin_strlen(str);
    for (u32 i = 0; i < length; ++i) {
        char c = str[i];
        switch (c) {
        case ' ':
            fn_710007f5ac(out, '_');
            break;
        case '(':
            fn_710007f564(out, "__PARI__");
            break;
        case ')':
            fn_710007f564(out, "__PARD__");
            break;
        case '-':
            fn_710007f564(out, "__DASH__");
            break;
        case '[':
            fn_710007f564(out, "__CORI__");
            break;
        case ']':
            fn_710007f564(out, "__CORD__");
            break;
        case '_':
            fn_710007f564(out, "__UNDS__");
            break;
        default:
            fn_710007f5ac(out, c);
            break;
        }
    }
}
