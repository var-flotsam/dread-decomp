// In the CKraidAcidBlobsAttack unit: an override that calls its base (fn_71005f1fb4), then keeps the
// argument at +0x30 when it is a CCharClassKraidAcidBlobsAttack and picks a random index below its
// count at +0x118 (0 when the count is 0) into +0x44. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2772.h"
#include "types.h"

using base::reflection::CClass;

struct UnkKraidAcidBlobsDef {
    virtual const CClass* vf_0x0();
    u8 pad_0x8[0x110];
    u32 count;
};

struct UnkKraidAcidBlobsAttack {
    u8 pad_0x0[0x30];
    UnkKraidAcidBlobsDef* def;
    u8 pad_0x38[0xc];
    u32 index;
};

void fn_71005f1fb4(char* s, base::reflection::UnkReflObject* a);

void fn_71005f122c(UnkKraidAcidBlobsAttack* self, UnkKraidAcidBlobsDef* def) {
    fn_71005f1fb4(reinterpret_cast<char*>(self), reinterpret_cast<base::reflection::UnkReflObject*>(def));
    if (!def)
        return;
    UnkKraidAcidBlobsDef* cast = fn_710009d50c(def->vf_0x0(), fn_71005d6288()) ? def : nullptr;
    self->def = cast;
    u32 count = cast->count;
    self->index = count != 0 ? (fn_71012395a0() & 0x7fff) % count : 0;
}
