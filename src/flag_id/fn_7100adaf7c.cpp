// Builds a typed value whose type comes from a reflection getter.
#include "types.h"

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

struct UnkType;
const UnkType* fn_7100005178();
void fn_71000a18d4(UnkValue_0x18* out, const UnkType* type, void* data, bool flag);

UnkValue_0x18 fn_7100adaf7c(void* self, const UnkHolder* holder) {
    void* data = holder->payload;
    if (holder->payload->kind == 2)
        data = holder->payload->data;
    const UnkType* type = fn_7100005178();
    UnkValue_0x18 result;
    fn_71000a18d4(&result, type, data, true);
    return result;
}
