// Unit 82 (base::reflection): builds a reflected value's argument (fn_71000a1710) and forwards it,
// with the object at +0x8 and the caller's argument, to fn_710009da40, returning its result.
// Names and layouts are placeholders.
#include "types.h"

struct UnkReflValueArg;
struct UnkReflValue;
void fn_71000a1710(UnkReflValueArg* out, const UnkReflValue* value);

struct UnkReflForwardResult {
    void* field_0x0;
    UnkReflForwardResult(const UnkReflForwardResult& o);
};

UnkReflForwardResult fn_710009da40(void* object, UnkReflValueArg* arg, void* extra);

struct UnkReflForwardValue {
    u8 pad_0x0[0x8];
    void* object;  // 0x8
};

UnkReflForwardResult fn_71000a20f4(const UnkReflForwardValue* self, void* extra) {
    void* object = self->object;
    alignas(8) u8 arg[0x10];
    fn_71000a1710(reinterpret_cast<UnkReflValueArg*>(arg), reinterpret_cast<const UnkReflValue*>(self));
    return fn_710009da40(object, reinterpret_cast<UnkReflValueArg*>(arg), extra);
}
