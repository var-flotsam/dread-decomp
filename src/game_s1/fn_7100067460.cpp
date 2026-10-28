// Wide-string counterpart of fn_710007fb9c: whether a wide string (data pointer at 8) contains a
// substring. Its only caller is fn_7101185904.

#include "base/Char32.h"
#include "types.h"

struct UnkWideStr_7100067460 {
    u64 field_0x0;
    const char32_t* data;
};

bool fn_7100067460(const UnkWideStr_7100067460* self, const char32_t* str) {
    return self->data && char32_strstr(self->data, str);
}
