// From unit 55 of tools/tu_map.py (0x710007b430-0x710007bb30): sets a heap copy of a C string held
// in one pointer. The same text keeps the copy; otherwise the old copy is freed and the new one
// taken from the default allocator (flags 0x11), null when empty. A null string frees the old
// copy but leaves the pointer as it was (fn_710007b8f0). Names are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "types.h"

void fn_710007b8f0(char** self, const char* str) {
    base::UnkAllocator* allocator = fn_7100083990();
    char* old = *self;
    if (!str) {
        if (old)
            allocator->vf_0x48(old);
        return;
    }
    if (old == str)
        return;
    if (old) {
        if (strcmp(str, old) == 0)
            return;
        allocator->vf_0x48(old);
    }
    u32 length = strlen(str);
    if (length == 0) {
        *self = nullptr;
        return;
    }
    char* copy = static_cast<char*>(allocator->vf_0x40(length + 1, 8, 0x11));
    *self = copy;
    memcpy(copy, str, length);
    (*self)[length] = '\0';
}
