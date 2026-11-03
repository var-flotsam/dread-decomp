// From unit 41 of tools/tu_map.py (base::global::timeline::CEvent): equality and inequality of the
// localized text of src/base/global/UnkWideTextMethods.cpp (the UTF-32 characters at +0x8, their count
// at +0x10): equal counts and either the same buffer or two non-null buffers that compare equal.
// Names and layouts are placeholders.
#include <stddef.h>

#include "types.h"

extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);

struct UnkWideTextCmp {
    void* vtable;
    wchar_t* chars;
    u32 count;
};

static inline bool Equals(const UnkWideTextCmp* a, const UnkWideTextCmp* b) {
    if (a->count != b->count)
        return false;
    const wchar_t* x = a->chars;
    const wchar_t* y = b->chars;
    if (x == y)
        return true;
    if (!x || !y)
        return false;
    return wcscmp(x, y) == 0;
}

bool fn_7100067130(const UnkWideTextCmp* self, const UnkWideTextCmp* other) {
    return Equals(self, other);
}

bool fn_71000671a0(const UnkWideTextCmp* self, const UnkWideTextCmp* other) {
    return !Equals(self, other);
}
