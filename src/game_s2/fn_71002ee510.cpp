// Builds a result (kind 2, empty) in place and hands it the playthrough definition of the
// owner's object at 0x70 (when it is one) to fill in.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkDefObject {
    virtual const CClass* vf_0x0();
};
struct UnkDefOwner {
    u8 unk_0x0[0x70];
    UnkDefObject* object;
};
struct UnkDefHolder {
    u8 unk_0x0[8];
    UnkDefOwner* owner;
};

const CType* fn_71002ee1dc();

UnkReflValue fn_71002ee510(void* unused, UnkDefHolder* holder) {
    UnkDefObject* object = holder->owner->object;
    UnkDefObject* def = object && fn_710009d50c(object->vf_0x0(), fn_71002effc4()) ? object : nullptr;
    UnkReflValue result;
    const CType* type = fn_71002ee1dc();
    result.kind = 2;
    result.field_0x0 = nullptr;
    result.type = nullptr;
    fn_71000a18d4(&result, type, &def, 0);
    return result;
}
