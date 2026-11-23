// CCharClassMovementComponent: the constructor body fn_710097b250, fn_710097b304 without the
// class-entry pass. Names and layouts are placeholders.
#include "game/CCharClassComponent.h"
#include "game/CCharClassMovementComponent.h"
#include "game/UnkCharClassMovementComponent.h"
#include "types.h"

struct UnkStruct_71008bca30;

void fn_710097b250(UnkCharClassMovementComponent* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = g_unk_7101a85840 + 2;
    self->field_0x58 = true;
    self->field_0x18 = 4000;
    self->field_0x5c = 0;
}
