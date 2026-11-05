// CEmmyAIComponent unit (tools/tu_map.py) and its copy 0x7100708034 in an unnamed unit: forwards all nine
// arguments (the last one passed on the stack) to the receiver's vtable slot 0x560. Names and the argument
// types are placeholders.
#include "types.h"

struct UnkForwardTarget_b54 {
    void** vtable;
};

typedef void (*UnkForward560_b54)(UnkForwardTarget_b54*, void*, void*, void*, void*, void*, void*, void*, u32);

void fn_7100569fd8(UnkForwardTarget_b54* self, void* a1, void* a2, void* a3, void* a4, void* a5, void* a6,
                   void* a7, u32 a8) {
    reinterpret_cast<UnkForward560_b54>(self->vtable[0x560 / 8])(self, a1, a2, a3, a4, a5, a6, a7, a8);
}

void fn_7100708034(UnkForwardTarget_b54* self, void* a1, void* a2, void* a3, void* a4, void* a5, void* a6,
                   void* a7, u32 a8) {
    reinterpret_cast<UnkForward560_b54>(self->vtable[0x560 / 8])(self, a1, a2, a3, a4, a5, a6, a7, a8);
}
