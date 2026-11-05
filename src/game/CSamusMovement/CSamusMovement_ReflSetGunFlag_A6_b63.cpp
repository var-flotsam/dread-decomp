// CSamusMovement's unit (tools/tu_map.py, the name from reflection): a reflected function that reads a bool from
// the argument's stream (fn_710007cf30, "b") and, when the owner's (+0x30 of the movement at +8) component at
// +0x2a8 is a CSamusGunComponent, passes it to fn_710085b76c; it returns the int 1. Names are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

void fn_710085b76c(UnkReflObject* gun, bool value);

struct UnkGunOwner_A6_b63 {
    u8 pad_0x0[0x2a8];
    UnkReflObject* gun;
};

struct UnkSamusMovement_A6_b63 {
    u8 pad_0x0[0x30];
    UnkGunOwner_A6_b63* owner;
};

static inline __attribute__((always_inline)) void* ArgPtr_A6s_b63(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71009f0fc4(char* obj, UnkReflValue* const* args) {
    UnkSamusMovement_A6_b63* self = *reinterpret_cast<UnkSamusMovement_A6_b63**>(obj + 8);
    void* stream = ArgPtr_A6s_b63(args[0]);
    u8 value = 0;
    s32 one;
    fn_710007cf30(stream, "b", &value);
    u8 flag = value;
    if (UnkReflObject* gun = self->owner->gun) {
        if (fn_710009d50c(gun->vf_0x0(), reinterpret_cast<const CType*>(fn_7100857564())))
            fn_710085b76c(gun, flag != 0);
    }
    one = 1;
    const CType* type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &one, false);
    return r;
}
