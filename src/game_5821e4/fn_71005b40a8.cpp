// CHydrogigaBraidAttack's deleting destructor: its listener (+0x70), then CHydrogigaAttack's
// part (vtable store, the string-id array at +0x28), CAttack's, and the delete.
#include "CAttackDtor.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"

extern void* g_unk_7101a505c8[];  // CHydrogigaBraidAttack's vtable
extern void* g_unk_7101a504d0[];  // CHydrogigaAttack's vtable

struct UnkAttack_71005b40a8 {
    void** vtable;
    u8 pad_0x8[0x20];
    base::UnkVector<base::UnkStrIdMember> field_0x28;
    u8 pad_0x50[0x20];
    UnkAttackListenerRaw field_0x70;
};

void fn_71005b40a8(UnkAttack_71005b40a8* self) {
    self->vtable = g_unk_7101a505c8 + 2;
    self->field_0x70.Destroy();
    self->vtable = g_unk_7101a504d0 + 2;
    self->field_0x28.Free();
    fn_71003606c0(self);
    operator delete(self);
}
