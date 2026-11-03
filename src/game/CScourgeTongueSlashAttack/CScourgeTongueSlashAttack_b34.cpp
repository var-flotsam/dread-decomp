#include "types.h"

struct UnkScourgeTongue;
bool fn_710069a7e4(UnkScourgeTongue* self, void* a, void* b, u32 kind, u32* outIndex, f32* outValue);

// fn_710069a7e4 of kind 1, discarding its outputs.
bool fn_710069a7ac(UnkScourgeTongue* self, void* a, void* b) {
    u32 index = 0;
    f32 value = -1.0f;
    return fn_710069a7e4(self, a, b, 1, &index, &value);
}
