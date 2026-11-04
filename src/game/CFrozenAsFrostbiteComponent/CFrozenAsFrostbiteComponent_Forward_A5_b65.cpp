// CFrozenAsFrostbiteComponent's unit: forwards to the virtual at slot 0x200. Names are placeholders.
#include "types.h"

struct UnkFrostbiteVirtual_A5_b65 {
    virtual void vf_0x0();
};

typedef void (*UnkFrostbiteFn_A5_b65)(UnkFrostbiteVirtual_A5_b65*, void*, void*);

void fn_71007f5b7c(UnkFrostbiteVirtual_A5_b65* self, void* a, void* b) {
    (*reinterpret_cast<UnkFrostbiteFn_A5_b65**>(self))[0x200 / 8](self, a, b);
}
