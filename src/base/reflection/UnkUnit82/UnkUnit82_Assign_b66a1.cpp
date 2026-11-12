// Unit 82 (base::reflection): assigns one reflected value to another when both have a type at +0x8,
// building each one's argument with fn_71000a1710 and converting through fn_710009d5d4. Names and
// layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/reflection.h"
#include "types.h"

#include "base/reflection.h"
bool fn_710009d5d4(const char* type, base::reflection::UnkTypedRef* dst, const base::reflection::UnkTypedRef* src);

struct UnkReflAssignValue82_b66a1 {
    u8 pad_0x0[0x8];
    void* type;  // 0x8
};

bool fn_71000a1ed4(const UnkReflAssignValue82_b66a1* self, const UnkReflAssignValue82_b66a1* other) {
    if (self->type == nullptr || other->type == nullptr) {
        return false;
    }
    alignas(8) u8 to[0x10];
    alignas(8) u8 from[0x10];
    fn_71000a1710(reinterpret_cast<UnkReflValueArg*>(to), reinterpret_cast<const base::reflection::UnkReflValue*>(self));
    fn_71000a1710(reinterpret_cast<UnkReflValueArg*>(from), reinterpret_cast<const base::reflection::UnkReflValue*>(other));
    return fn_710009d5d4(static_cast<const char*>(self->type), reinterpret_cast<base::reflection::UnkTypedRef*>(to),
                         reinterpret_cast<const base::reflection::UnkTypedRef*>(from));
}
