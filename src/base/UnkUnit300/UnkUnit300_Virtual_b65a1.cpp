// Unit 300 of tools/tu_map.py (no reflected class): returns what the receiver's own virtual at slot
// 0x48 returns, a value larger than 16 bytes (returned through x8). Names and sizes are placeholders.
#include "types.h"

struct UnkUnit300Result_b65a1 {
    u64 words[3];
};

struct UnkUnit300Object_b65a1 {
    void** vtable;
};

UnkUnit300Result_b65a1 fn_7100211a84(UnkUnit300Object_b65a1* self) {
    typedef UnkUnit300Result_b65a1 (*Fn)(UnkUnit300Object_b65a1*);
    return reinterpret_cast<Fn>(self->vtable[0x48 / 8])(self);
}
