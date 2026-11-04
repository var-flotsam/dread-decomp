// CCharacterMovement's unit: forwards to the virtuals at slots 0x298 and 0x2a0. Names are placeholders.
#include "types.h"

struct UnkCharMoveVirtual_A5_b65 {
    virtual void vf_0x0();
};

typedef void (*UnkCharMoveFn7_A5_b65)(UnkCharMoveVirtual_A5_b65*, void*, void*, void*, void*, void*, void*);
typedef void (*UnkCharMoveFn6_A5_b65)(UnkCharMoveVirtual_A5_b65*, void*, void*, void*, void*, void*);

void fn_710097a838(UnkCharMoveVirtual_A5_b65* self, void* a, void* b, void* c, void* d, void* e, void* f) {
    (*reinterpret_cast<UnkCharMoveFn7_A5_b65**>(self))[0x298 / 8](self, a, b, c, d, e, f);
}

void fn_710097a844(UnkCharMoveVirtual_A5_b65* self, void* a, void* b, void* c, void* d, void* e) {
    (*reinterpret_cast<UnkCharMoveFn6_A5_b65**>(self))[0x2a0 / 8](self, a, b, c, d, e);
}
