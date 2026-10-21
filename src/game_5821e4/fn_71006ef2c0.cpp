// Destructor (fn_71006ef2c0) and deleting destructor (fn_71006ef388) of an object that embeds
// two attacks (+0x146c8, +0x146f8; their destructors store no vtable), two ref-counted pointers
// (+0x14770, +0x14778) and a member destroyed out of line (+0x147c8, fn_71002b6094); then the
// base's destructor (fn_7100369380).
#include "CAttackDtor.h"

extern void* g_unk_7101a6ae18[];  // the object's vtable

void fn_7100369380(void* self);
void fn_71002b6094(void* p);

struct UnkRefPtrRaw_71006ef2c0 {
    UnkAttackOwner* ptr;

    void Release() {
        if (UnkAttackOwner* p = ptr) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};

struct UnkOwner_71006ef2c0 {
    void** vtable;
    u8 pad_0x8[0x146c0];
    u8 field_0x146c8[0x30];
    u8 field_0x146f8[0x78];
    UnkRefPtrRaw_71006ef2c0 field_0x14770;
    UnkRefPtrRaw_71006ef2c0 field_0x14778;
    u8 pad_0x14780[0x48];
    u8 field_0x147c8[0x8];

    void Destroy() {
        vtable = g_unk_7101a6ae18 + 2;
        fn_71002b6094(field_0x147c8);
        field_0x14778.Release();
        field_0x14770.Release();
        fn_71003606c0(field_0x146f8);
        fn_71003606c0(field_0x146c8);
        fn_7100369380(this);
    }
};

void fn_71006ef2c0(UnkOwner_71006ef2c0* self) {
    self->Destroy();
}

void fn_71006ef388(UnkOwner_71006ef2c0* self) {
    self->Destroy();
    operator delete(self);
}
