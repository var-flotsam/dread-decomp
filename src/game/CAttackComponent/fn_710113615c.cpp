// Deleting destructor of an object with a secondary base at 0x40 and a member at 0x98 that
// owns three arrays: restores the vtables, clears the u16 at 0x48, frees the arrays, runs the
// base destructor (fn_71010c4194) and frees the object.
#include "base/global/CRntVector.h"
#include "game/CAttackComponent.h"
#include "game/UnkBase_0x40.h"
#include "types.h"

struct UnkSecondaryBase_0x58 {
    void** vtable;
    u16 field_0x8;
    u8 unk_0xa[0x4e];
};

struct UnkArrays_0x80 {
    void** vtable;
    base::global::CRntVector<u64> field_0x8;
    base::global::CRntVector<u64> field_0x30;
    base::global::CRntVector<u64> field_0x58;

    ~UnkArrays_0x80() {
        vtable = &g_unk_7101cb2ec0[2];
        field_0x58.Free();
        field_0x30.Free();
        field_0x8.Free();
    }
};

struct UnkObject_710113615c : UnkBase_0x40, UnkSecondaryBase_0x58 {
    UnkArrays_0x80 arrays;

    ~UnkObject_710113615c() {
        UnkBase_0x40::vtable = &g_unk_7101cb2cb8[2];
        UnkSecondaryBase_0x58::vtable = &g_unk_7101cb2cb8[0x3e];
        field_0x8 = 0;
    }
};

void fn_710113615c(UnkObject_710113615c& self) {
    delete &self;
}
