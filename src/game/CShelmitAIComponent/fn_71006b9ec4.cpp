// Destructor (fn_71006b9ec4) and deleting destructor (fn_71006b9f94) of an object that embeds a
// CShelmitPlasmaRayAttack at +0x146d0 and a ref-counted pointer at +0x14788: the pointer is
// released, then the attack's destructor (its listener at +0x38, then CAttack's), then the
// base's (fn_7100369380).
#include "../../game_5821e4/CAttackDtor.h"

extern void* g_unk_7101a63bc0[];  // the object's vtable
extern void* g_unk_7101a637a8[];  // CShelmitPlasmaRayAttack's vtable

void fn_7100369380(void* self);

struct UnkAttack_71006b9ec4 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkAttackListenerRaw field_0x38;

    void Destroy() {
        vtable = g_unk_7101a637a8 + 2;
        field_0x38.Destroy();
        fn_71003606c0(this);
    }
};

struct UnkOwner_71006b9ec4 {
    void** vtable;
    u8 pad_0x8[0x146c8];
    UnkAttack_71006b9ec4 field_0x146d0;
    u8 pad_0x14728[0x60];
    UnkAttackOwner* field_0x14788;

    void Destroy() {
        vtable = g_unk_7101a63bc0 + 2;
        if (UnkAttackOwner* p = field_0x14788) {
            if (--p->ref_count == 0)
                p->Delete();
        }
        field_0x146d0.Destroy();
        fn_7100369380(this);
    }
};

void fn_71006b9ec4(UnkOwner_71006b9ec4* self) {
    self->Destroy();
}

void fn_71006b9f94(UnkOwner_71006b9ec4* self) {
    self->Destroy();
    operator delete(self);
}
