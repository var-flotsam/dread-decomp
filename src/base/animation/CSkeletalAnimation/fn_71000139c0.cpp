// Destructors (complete and deleting) of a class (vtable unk_7101a19380) holding a counted
// object at 8 (count at +0x54, freed by fn_710001737c when it drops to zero), as fn_710019e82c
// does at 0x10: fn_71000139c0 and fn_71000139f0. Called only through the vtable.

#include <new>

#include "base/UnkCountedResource.h"
#include "types.h"

extern char unk_7101a19380[];

struct UnkCountedRef_71000139c0 {
    char* vtable;
    UnkCountedResource* counted;
};

__attribute__((always_inline)) static inline void Destroy(UnkCountedRef_71000139c0* self) {
    self->vtable = unk_7101a19380 + 0x10;
    if (UnkCountedResource* counted = self->counted) {
        if (--counted->ref_count == 0)
            fn_710001737c(counted);
    }
}

void fn_71000139c0(UnkCountedRef_71000139c0* self) { Destroy(self); }

void fn_71000139f0(UnkCountedRef_71000139c0* self) {
    Destroy(self);
    operator delete(self);
}
