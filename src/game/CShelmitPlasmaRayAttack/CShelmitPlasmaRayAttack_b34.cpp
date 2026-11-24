#include "types.h"

struct CShelmitPlasmaRayAttack_b34 {
    u8 unk_0x0[0x30];
    u32 state;
};

void CShelmitPlasmaRayAttack_RemovePlasmaRayColliderAndEffects(CShelmitPlasmaRayAttack_b34* self);

// Ends the attack: states 0-2 and 7-9 just move to 11; states 3-6 also run fn_71006b4900.
void fn_71006b6508(CShelmitPlasmaRayAttack_b34* self) {
    switch (self->state) {
    case 3:
    case 4:
    case 5:
    case 6:
        self->state = 11;
        CShelmitPlasmaRayAttack_RemovePlasmaRayColliderAndEffects(self);
        break;
    case 0:
    case 1:
    case 2:
        self->state = 11;
        break;
    case 7:
    case 8:
    case 9:
        self->state = 11;
        break;
    default:
        break;
    }
}
