// The string pool's buffer compared with a C string: true when they differ. The buffer is
// a pointer and a 32-bit length; a null or empty string equals any other null or empty one.
#include <string.h>

#include "base/global/UnkStringPool.h"

namespace base::global {

struct UnkStringBuffer {
    const char* str;
    u32 length;
};

// Inlined: strcmp that accepts null pointers. Null and "" sort after any other string.
inline int UnkSafeStrcmp(const char* a, const char* b) {
    bool a_empty = !a || !*a;
    bool b_empty = !b || !*b;
    if (a_empty || b_empty)
        return a_empty ? !(a_empty && b_empty) : -1;
    return strcmp(a, b);
}

}  // namespace base::global

// NON_MATCHING(minor): the original computes the empty-string result with a select before
// testing `a_empty || b_empty` (tbnz) and shares the final `!= 0` with the equal-pointer case;
// we branch first (cmp #1, b.eq) and return directly. Same blocks and calls otherwise.
bool fn_710007f240(char* buffer, const char* str) {
    auto* self = reinterpret_cast<base::global::UnkStringBuffer*>(buffer);
    if (self->length != (u32)(str ? strlen(str) : 0))
        return true;
    if (self->str == str)
        return false;
    return base::global::UnkSafeStrcmp(self->str, str) != 0;
}
