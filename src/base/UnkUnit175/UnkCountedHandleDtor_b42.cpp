// The destructor of a handle holding a counted resource (base/UnkCountedResource.h), one copy per
// unit: the system fonts' unit registers the first with __cxa_atexit for its two font handles
// (src/game_s1/fn_71001545d0.cpp); the others are CPlaythrough's and CPlaythroughDef's reflected
// destructors. Drops the reference, freeing the resource when it was the last. Names are placeholders.
#include "base/UnkCountedResource.h"
#include "types.h"

struct UnkCountedHandle_b42a1 {
    UnkCountedResource* ptr;

    __attribute__((always_inline)) void Release() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }
};

// The first's signature as src/game_s1/fn_71001545d0.cpp declares it.
void fn_7100154538(void* handle) { static_cast<UnkCountedHandle_b42a1*>(handle)->Release(); }
void fn_71002ed208(UnkCountedHandle_b42a1* self) { self->Release(); }
void fn_71002f4200(UnkCountedHandle_b42a1* self) { self->Release(); }
