// CSluggerSpitAttack's destructor: its ref-counted pointer (+0xe0), the two pointers of the
// member at +0x88, its listener (+0x38), then CAttack's.
#include "../../game_5821e4/CAttackDtor.h"

extern void* g_unk_7101a64db0[];  // CSluggerSpitAttack's vtable

struct UnkAttack_71006c3a64 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkAttackListenerRaw field_0x38;
    u8 pad_0x58[0x30];
    UnkAttackRefPtrRaw field_0x88;
    UnkAttackRefPtrRaw field_0x90;
    u8 pad_0x98[0x48];
    UnkAttackRefPtrRaw field_0xe0;
};

void fn_71006c3a64(UnkAttack_71006c3a64* self) {
    self->vtable = g_unk_7101a64db0 + 2;
    self->field_0xe0.Release();
    self->field_0x90.Release();
    self->field_0x88.Release();
    self->field_0x38.Destroy();
    fn_71003606c0(self);
}
