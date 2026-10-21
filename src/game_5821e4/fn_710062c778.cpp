// Destructor of an object that embeds an attack at +0x146c8 and a member at +0x147f8 (destroyed
// out of line, fn_7100d62bd8); then the base's (fn_7100369380). The attack's destructor stores no
// vtable: it frees its array at +0xe8, clears the count at +0xa8 and destroys its listener
// (+0x70); its base (vtable 0x7101a57430) destroys a second listener (+0x30), then CAttack's.
#include "CAttackDtor.h"
#include "base/UnkVector.h"

extern void* g_unk_7101a576f8[];  // the object's vtable
extern void* g_unk_7101a57430[];  // the attack's base's vtable

void fn_7100369380(void* self);
void fn_7100d62bd8(void* p);

struct UnkAttack_710062c778 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkAttackListenerRaw field_0x30;
    u8 pad_0x50[0x20];
    UnkAttackListenerRaw field_0x70;
    u8 pad_0x90[0x18];
    u32 field_0xa8;
    u8 pad_0xac[0x3c];
    base::UnkVector<u64> field_0xe8;

    __attribute__((always_inline)) void Destroy() {
        field_0xe8.Free();
        field_0xa8 = 0;
        field_0x70.Destroy();
        vtable = g_unk_7101a57430 + 2;
        field_0x30.Destroy();
        fn_71003606c0(this);
    }
};

struct UnkOwner_710062c778 {
    void** vtable;
    u8 pad_0x8[0x146c0];
    UnkAttack_710062c778 field_0x146c8;
    u8 pad_0x147d8[0x20];
    u8 field_0x147f8[0x8];

    __attribute__((always_inline)) void Destroy() {
        vtable = g_unk_7101a576f8 + 2;
        fn_7100d62bd8(field_0x147f8);
        field_0x146c8.Destroy();
        fn_7100369380(this);
    }
};

void fn_710062c778(UnkOwner_710062c778* self) {
    self->Destroy();
}
