#include "types.h"

u64 fn_7100308c74();

struct UnkUnit435Self;
void fn_710033cd50(UnkUnit435Self* self, u32 value);

struct UnkUnit435Other {
    u8 unk_0x0[0xc8];
    u64* id;
};

void fn_710033e41c(UnkUnit435Self* self, UnkUnit435Other* other) {
    if (other) {
        u64* id = other->id;
        u64* current = (u64*)fn_7100308c74();
        if (__builtin_expect(*id == *current, 0))
            fn_710033cd50(self, 1);
    }
}
