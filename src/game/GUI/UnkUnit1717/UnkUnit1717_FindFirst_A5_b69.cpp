// Unit 1717 (GUI area): two searches over a pointer vector (data at +0x8, count at +0x10) that return
// the first non-null result of a lookup on each element (fn_7100b06c00 and fn_7100b0694c), or null.
// The vector is re-read after every lookup. Names and layouts are placeholders.
#include "types.h"

void* fn_7100b06c00(void* element, void* key);
void* fn_7100b0694c(void* element, void* key);

struct UnkUnit1717Vector_A5_b69 {
    u8 pad_0x0[0x8];
    void** data;
    u32 count;

    void** begin() { return data; }
    void** end() { return data + count; }
};

void* fn_7100b774e4(UnkUnit1717Vector_A5_b69* self, void* key) {
    for (void** it = self->begin(); it != self->end(); ++it)
        if (void* found = fn_7100b06c00(*it, key))
            return found;
    return nullptr;
}

void* fn_7100b77554(UnkUnit1717Vector_A5_b69* self, void* key) {
    for (void** it = self->begin(); it != self->end(); ++it)
        if (void* found = fn_7100b0694c(*it, key))
            return found;
    return nullptr;
}
