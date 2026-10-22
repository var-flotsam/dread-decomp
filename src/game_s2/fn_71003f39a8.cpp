// Destructor of CBigkranXSpitAttack: frees the array at +0x100, releases the ref-counted
// pointers at +0xf8, +0x90 and +0x88, then destroys the embedded listener (+0x38) and CAttack's
// part (fn_71003606c0).
#include "../game_5821e4/CAttackDtor.h"
#include "base/UnkVector.h"

extern void* g_unk_7101a31348[];  // the class's vtable

struct UnkSpitAttackDtor {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkAttackListenerRaw field_0x38;
    u8 pad_0x58[0x30];
    UnkAttackRefPtrRaw field_0x88;
    UnkAttackRefPtrRaw field_0x90;
    u8 pad_0x98[0x60];
    UnkAttackRefPtrRaw field_0xf8;
    base::UnkVector<void*> field_0x100;

    __attribute__((always_inline)) void Destroy() {
        vtable = g_unk_7101a31348 + 2;
        field_0x100.Free();
        field_0xf8.Release();
        field_0x90.Release();
        field_0x88.Release();
        field_0x38.Destroy();
        fn_71003606c0(this);
    }
};

void fn_71003f39a8(UnkSpitAttackDtor* self) {
    self->Destroy();
}
