// Builds a typed value whose type comes from a reflection getter.
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkPayload_adaf7c {
    void* data;
    u8 unk_0x8[0x8];
    s32 kind;
};

struct UnkHolder_adaf7c {
    u8 unk_0x0[0x8];
    UnkPayload_adaf7c* payload;
};

UnkReflValue fn_7100adaf7c(void* self, const UnkHolder_adaf7c* holder) {
    void* data = holder->payload;
    if (holder->payload->kind == 2)
        data = holder->payload->data;
    const CType* type = fn_7100005178();
    UnkReflValue result;
    result.field_0x0 = nullptr;
    result.type = nullptr;
    result.kind = 2;
    fn_71000a18d4(&result, type, data, true);
    return result;
}
