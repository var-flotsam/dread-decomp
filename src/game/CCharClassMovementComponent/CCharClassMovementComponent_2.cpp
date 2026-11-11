// CCharClassMovementComponent: the constructor body fn_710097b250, fn_710097b304 without the
// class-entry pass. Names and layouts are placeholders.
#include "game/CCharClassComponent.h"
#include "types.h"

struct UnkStruct_71008bca30;
extern void* g_unk_7101a85840[];

struct UnkCharClassMovementComponent {
    void** vtable;
    u8 pad_0x8[0x10];
    u64 field_0x18;
    u8 pad_0x20[0x38];
    bool field_0x58;
    u8 pad_0x59[3];
    u32 field_0x5c;
};

void fn_710097b250(UnkCharClassMovementComponent* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = g_unk_7101a85840 + 2;
    self->field_0x58 = true;
    self->field_0x18 = 4000;
    self->field_0x5c = 0;
}
