// CPowerBeamGun's unit: forwards to the virtual at slot 0x158 with mode 3, once plainly and once after
// setting the flag at +0x1880. Names are placeholders.
#include "types.h"

struct UnkPowerBeamVirtual_A5_b65 {
    virtual void vf_0x0();
};

typedef void (*UnkPowerBeamFn_A5_b65)(UnkPowerBeamVirtual_A5_b65*, void*, void*, u32, void*);

void fn_710085477c(UnkPowerBeamVirtual_A5_b65* self, void* a, void* b, u32 unused, void* d) {
    (*reinterpret_cast<UnkPowerBeamFn_A5_b65**>(self))[0x158 / 8](self, a, b, 3, d);
}

void fn_710085478c(UnkPowerBeamVirtual_A5_b65* self, void* a, void* b, u32 unused, void* d) {
    reinterpret_cast<bool*>(self)[0x1880] = true;
    (*reinterpret_cast<UnkPowerBeamFn_A5_b65**>(self))[0x158 / 8](self, a, b, 3, d);
}
