// CAnimationComponent unit: looks an entry up by key with fn_7101176d34 and stores it through `out` when
// asked; says whether it was found. Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "types.h"

struct UnkStruct_7101176ea8;

bool fn_7101176e78(UnkStruct_7101176ea8* self, void* key, void** out) {
    void* entry = fn_7101176d34(self, key);
    if (!entry)
        return false;
    if (out)
        *out = entry;
    return true;
}
