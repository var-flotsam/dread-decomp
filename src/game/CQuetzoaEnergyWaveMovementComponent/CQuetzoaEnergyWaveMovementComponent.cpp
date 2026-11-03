#include "types.h"

struct UnkComponent_0x5d0;
void fn_7100ac3184(UnkComponent_0x5d0* self);

struct CQuetzoaEnergyWaveMovementComponent {
    u8 unk_0x0[0x5d8];
    void* field_0x5d8;
    void* field_0x5e0;
    void* field_0x5e8;
};

// Runs the projectile movement's update, then clears the three wave fields (last first).
void fn_710064c664(CQuetzoaEnergyWaveMovementComponent* self) {
    fn_7100ac3184(reinterpret_cast<UnkComponent_0x5d0*>(self));
    self->field_0x5e8 = nullptr;
    self->field_0x5e0 = nullptr;
    self->field_0x5d8 = nullptr;
}
