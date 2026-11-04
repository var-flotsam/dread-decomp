// Unit 346 (base::reflection): collection slots of a reflected fixed array of three 0xc8-byte
// values (constructed by fn_710025b068, assigned by fn_710025b7f4, destroyed by fn_710025ad0c):
// slot 0x110 resets every value to a default-constructed one (fn_7100260f18). Names are placeholders.
#include "types.h"

void fn_710025b068(char* self);
void fn_710025b7f4(char* self, char* other);
void fn_710025ad0c(char*);  // the signature src/clones/clones_00.cpp declares

struct UnkFixedValue_b60 {
    char data[0xc8];
};

struct UnkFixedValues_b60 {
    UnkFixedValue_b60 values[3];
};

static inline __attribute__((always_inline)) void ResetValue_b60(UnkFixedValue_b60* value) {
    UnkFixedValue_b60 tmp;
    fn_710025b068(tmp.data);
    fn_710025b7f4(value->data, tmp.data);
    fn_710025ad0c(tmp.data);
}

void fn_7100260f18(UnkFixedValues_b60* self) {
    for (u32 i = 0; i < 3; ++i)
        ResetValue_b60(&self->values[i]);
}
