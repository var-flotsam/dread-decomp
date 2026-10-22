// Builds a result (kind 2, empty) in place and hands it the playthrough definition of the
// owner's object at 0x70 (when it is one) to fill in.
#include "types.h"

struct UnkReflType;
struct UnkDefObject {
    virtual const UnkReflType* vf_0x0();
};
struct UnkDefOwner {
    u8 unk_0x0[0x70];
    UnkDefObject* object;
};
struct UnkDefHolder {
    u8 unk_0x0[8];
    UnkDefOwner* owner;
};
struct UnkResult_0x18 {
    u64 field_0x0;
    u64 field_0x8;
    u32 kind;
};

bool fn_710009d50c(const UnkReflType* type, const UnkReflType* base);
const UnkReflType* fn_71002effc4();  // CPlaythroughDef::refl_GetClass
void* fn_71002ee1dc();
void fn_71000a18d4(UnkResult_0x18* out, void* a1, UnkDefObject** def, u32 a3);

UnkResult_0x18 fn_71002ee510(void* unused, UnkDefHolder* holder) {
    UnkDefObject* object = holder->owner->object;
    UnkDefObject* def = object && fn_710009d50c(object->vf_0x0(), fn_71002effc4()) ? object : nullptr;
    UnkResult_0x18 result;
    void* a1 = fn_71002ee1dc();
    result.kind = 2;
    result.field_0x0 = 0;
    result.field_0x8 = 0;
    fn_71000a18d4(&result, a1, &def, 0);
    return result;
}
