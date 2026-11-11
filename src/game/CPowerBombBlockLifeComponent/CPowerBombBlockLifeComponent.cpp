// Constructor of a component in the CPowerBombBlockLifeComponent unit: the base constructor
// fn_710115f7bc, the vtable, the string-id list at +0x240 (fn_71008e4154), two small objects at
// +0x278 and +0x298 (each a vtable and cleared words), then the post-construction virtual call
// (slot 3). Names and layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/CLifeComponent.h"
#include "game/CPowerBombBlockLifeComponent.h"
#include "game/CPullOffGrapplePointComponent.h"
#include "types.h"

void fn_71008e4154(char* self);

struct UnkPowerBombBlockSlotA {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkPowerBombBlockSlotB {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkPowerBombBlockLifeComponent {
    void** vtable;
    u8 pad_0x8[0x238];
    u8 field_0x240[0x38];
    UnkPowerBombBlockSlotA field_0x278;
    UnkPowerBombBlockSlotB field_0x298;
};

void fn_71008e43a8(UnkPowerBombBlockLifeComponent* self) {
    fn_710115f7bc(self);
    self->vtable = g_unk_7101a7f280 + 2;
    fn_71008e4154(reinterpret_cast<char*>(self->field_0x240));
    self->field_0x278.vtable = g_unk_7101a77000 + 2;
    self->field_0x278.field_0x8 = 0;
    self->field_0x278.field_0x10 = 0;
    self->field_0x278.field_0x18 = 0;
    self->field_0x298.vtable = g_unk_7101a22d48 + 2;
    self->field_0x298.field_0x8 = 0;
    self->field_0x298.field_0x10 = 0;
    self->field_0x298.field_0x18 = 0;
    reinterpret_cast<void (*)(UnkPowerBombBlockLifeComponent*)>(self->vtable[3])(self);
}
