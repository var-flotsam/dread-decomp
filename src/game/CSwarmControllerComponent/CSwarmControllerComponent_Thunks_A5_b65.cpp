// CSwarmControllerComponent's unit: small forwarders (virtual tail calls and a bound call of
// fn_710074d200). Names and layouts are placeholders.
#include "types.h"

struct UnkSwarmVirtual_A5_b65 {
    virtual void vf_0x0();
};

typedef void (*UnkSwarmFn3_A5_b65)(UnkSwarmVirtual_A5_b65*, void*, void*);

void fn_710074e594(UnkSwarmVirtual_A5_b65* self, void* a, void* b) {
    (*reinterpret_cast<UnkSwarmFn3_A5_b65**>(self))[0x188 / 8](self, a, b);
}

void fn_710074e5a0(UnkSwarmVirtual_A5_b65* self, void* a, void* b) {
    (*reinterpret_cast<UnkSwarmFn3_A5_b65**>(self))[0x1a0 / 8](self, a, b);
}

struct UnkSwarmTarget_A5_b65;
struct UnkSwarmBound_A5_b65 {
    void* owner;
    UnkSwarmTarget_A5_b65* field_0x8;
    void* field_0x10;
    u32 field_0x18;
    f32 field_0x1c;
};

void fn_710074d200(void* owner, void* arg, void* b, UnkSwarmTarget_A5_b65* target, bool flag, f32 value);

void fn_710074ea28(UnkSwarmBound_A5_b65* self, void* arg) {
    f32 value = self->field_0x1c;
    fn_710074d200(self->owner, arg, self->field_0x10, self->field_0x8, false, value);
}
