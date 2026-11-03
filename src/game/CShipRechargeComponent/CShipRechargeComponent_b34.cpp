#include "types.h"

struct CShipRechargeComponent {
    void** vtable;
    u8 unk_0x8[0x539 - 0x8];
    bool field_0x539;
    bool field_0x53a;
    bool field_0x53b;
};

void fn_7100aa2fd4(CShipRechargeComponent* self);

// Defers to fn_7100aa2fd4 until 0x539 is set; then, if 0x53b is set, calls slot 0x2b8 with true.
void fn_7100a8bf94(CShipRechargeComponent* self) {
    if (!self->field_0x539) {
        fn_7100aa2fd4(self);
        return;
    }
    if (self->field_0x53b)
        reinterpret_cast<void (*)(CShipRechargeComponent*, bool)>(self->vtable[0x2b8 / 8])(self, true);
}
