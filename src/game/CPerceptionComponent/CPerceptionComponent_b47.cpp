// CPerceptionComponent unit: after the base update, every perceiver in the list at +0x48 copies the owner's
// three sensing parameters. Names and layouts are placeholders.
#include "game/CComponent.h"
#include "types.h"

typedef u32 __attribute__((may_alias)) a_u32;

struct UnkParent;

struct UnkPerceptionParams {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0x54 - 0x48];
    u32 field_0x54;
    u8 pad_0x58[0x64 - 0x58];
    u32 field_0x64;
};

struct UnkPerceiver {
    u8 pad_0x0[0x8];
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
};

struct UnkPerceptionComponent {
    u8 pad_0x0[0x30];
    UnkPerceptionParams* field_0x30;
    u8 pad_0x38[0x48 - 0x38];
    UnkPerceiver** field_0x48;
    u8 pad_0x50[0x58 - 0x50];
    u32 field_0x58;
};

void fn_71009c8520(UnkPerceptionComponent* self) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    UnkPerceiver** it = self->field_0x48;
    UnkPerceiver** end = it + self->field_0x58;
    for (; it != end; ++it) {
        UnkPerceiver* p = *it;
        UnkPerceptionParams* params = self->field_0x30;
        u32 a = params->field_0x44;
        u32 b = params->field_0x54;
        u32 c = params->field_0x64;
        *(a_u32*)&p->field_0x8 = a;
        *(a_u32*)&p->field_0xc = b;
        *(a_u32*)&p->field_0x10 = c;
    }
}
