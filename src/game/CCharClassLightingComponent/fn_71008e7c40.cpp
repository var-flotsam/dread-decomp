// The CCharClassLightingComponent unit's copies of the energy wave's child loops
// (src/game/CAbilityEnergyWave/fn_71002fe508.cpp and fn_71002fff00.cpp): virtual slots 0x28 and 0x30 on
// each child of the array at +0x60 (fn_71008e7c40, fn_71008e7c80), and slot 0x18 with `dt` after the
// base update fn_71010c424c (fn_71008e7cc4).
#include "types.h"

struct UnkLightChild {
    void** vtable;
};

struct UnkLightParent {
    u8 pad_0x0[0x60];
    UnkLightChild** children;
    u32 count;
};

template <u32 Slot>
__attribute__((always_inline)) static inline void CallEach(UnkLightParent* self) {
    UnkLightChild** end = self->children + self->count;
    for (UnkLightChild** p = self->children; p != end; ++p)
        reinterpret_cast<void (*)(UnkLightChild*)>((*p)->vtable[Slot / 8])(*p);
}

void fn_71008e7c40(UnkLightParent* self) { CallEach<0x28>(self); }
void fn_71008e7c80(UnkLightParent* self) { CallEach<0x30>(self); }

// The base update (declared as src/game/CAbilityEnergyWave/fn_71002ffea8.cpp does).
struct UnkParent;
void fn_71010c424c(UnkParent* self);

void fn_71008e7cc4(UnkLightParent* self, float dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    UnkLightChild** end = self->children + self->count;
    for (UnkLightChild** p = self->children; p != end; ++p)
        reinterpret_cast<void (*)(UnkLightChild*, float)>((*p)->vtable[0x18 / 8])(*p, dt);
}
