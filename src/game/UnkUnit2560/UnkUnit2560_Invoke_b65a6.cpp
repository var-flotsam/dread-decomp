// Unit 2560: calls the callback stored at +0x38 (context at +0x30, or +0x48 with context +0x40) with two
// arguments; false when none is set. Names are placeholders.
#include "types.h"

struct UnkUnit2560Callback_b65a6 {
    void* context;
    bool (*function)(void* context, void* a, void* b);
};

struct UnkUnit2560_b65a6 {
    u8 pad_0x0[0x30];
    UnkUnit2560Callback_b65a6 first;
    UnkUnit2560Callback_b65a6 second;
};

bool fn_7100e1d234(UnkUnit2560_b65a6* self, void* a, void* b) {
    if (!self->first.function)
        return false;
    return self->first.function(self->first.context, a, b);
}

bool fn_7100e1d24c(UnkUnit2560_b65a6* self, void* a, void* b) {
    if (!self->second.function)
        return false;
    return self->second.function(self->second.context, a, b);
}
