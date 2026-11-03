// Complete destructors of the small class shape whose deleting destructors are in
// src/game/CAbilityOpticCamouflage/UnkReflValueHolderDtors.cpp (the same three units and vtables):
// the three pairs at +0x30..+0x60 clear themselves (last first), the class's vtable is set back and
// the reflected value at +0x18 is destroyed (fn_71000a1c60, a tail call). Names are placeholders.
#include "base/reflection/UnkReflCast.h"
#include "types.h"

extern char g_unk_7101a22fe8[];
extern char g_unk_7101a234f8[];
extern char g_unk_7101a239c8[];

struct UnkClearedPair_b42a1 {
    u64 a, b;

    ~UnkClearedPair_b42a1() {
        a = 0;
        b = 0;
    }
};

struct UnkReflValueHolder3_b42a1 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    UnkClearedPair_b42a1 pairs[3];

    __attribute__((always_inline)) void Destroy(char* vt) {
        for (int i = 2; i >= 0; --i)
            pairs[i].~UnkClearedPair_b42a1();
        vtable = vt + 0x10;
        fn_71000a1c60(&value);
    }
};

void fn_7100318278(UnkReflValueHolder3_b42a1* self) { self->Destroy(g_unk_7101a22fe8); }
void fn_71003283c4(UnkReflValueHolder3_b42a1* self) { self->Destroy(g_unk_7101a234f8); }
void fn_710033f41c(UnkReflValueHolder3_b42a1* self) { self->Destroy(g_unk_7101a239c8); }
