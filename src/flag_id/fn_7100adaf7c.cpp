// Builds a typed value whose type comes from a reflection getter.
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::reflection::CType;

struct UnkValue_0x18 {
    u64 field_0x0 = 0;
    u64 field_0x8 = 0;
    u32 kind = 2;
};

struct UnkPayload {
    void* data;
    u8 unk_0x8[0x8];
    s32 kind;
};

struct UnkHolder {
    u8 unk_0x0[0x8];
    UnkPayload* payload;
};

void fn_71000a18d4(UnkValue_0x18* out, const CType* type, void* data, bool flag);

UnkValue_0x18 fn_7100adaf7c(void* self, const UnkHolder* holder) {
    void* data = holder->payload;
    if (holder->payload->kind == 2)
        data = holder->payload->data;
    const CType* type = fn_7100005178();
    UnkValue_0x18 result;
    fn_71000a18d4(&result, type, data, true);
    return result;
}
