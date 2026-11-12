#include "types.h"

struct UnkScourgeTongue;
bool fn_710069a7e4(UnkScourgeTongue* self, void* arg, u32 mode, bool check, u32* outIndex, f32* outValue);

// fn_710069a7e4 with its check on, discarding its outputs.
bool fn_710069a7ac(UnkScourgeTongue* self, void* arg, u32 mode) {
    u32 index = 0;
    f32 value = -1.0f;
    return fn_710069a7e4(self, arg, mode, true, &index, &value);
}
