#include "types.h"

// Queries a distance into `out` (initialised to -1) and reports whether it was found and exceeds 10.
bool fn_71010fc778(void* self, f32* out);

bool fn_71006d5d68(void* self) {
    f32 dist = -1.0f;
    bool found = fn_71010fc778(self, &dist);
    return found & (dist > 10.0f);
}
