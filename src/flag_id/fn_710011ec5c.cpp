// Builds a typed value from an object's payload, using a global type descriptor.
#include "types.h"

struct UnkValue_0x18 {
    u64 field_0x0 = 0;
    u64 field_0x8 = 0;
    u32 kind = 2;
};

struct UnkPayload {
    u8 unk_0x0[0x8];
    void* data;
    u8 unk_0x10[0x40];
    s32 kind;
};

struct UnkHolder {
    u8 unk_0x0[0x8];
    UnkPayload* payload;
};

struct UnkType;
extern const UnkType g_unk_7101cc2e90;

void fn_71000a18d4(UnkValue_0x18* out, const UnkType* type, void* data, bool flag);

// NON_MATCHING(minor): one temporary lands in w8 instead of w9 (register allocation only).
UnkValue_0x18 fn_710011ec5c(void* self, const UnkHolder* holder) {
    void* data = &holder->payload->data;
    if (holder->payload->kind == 2)
        data = *static_cast<void**>(data);
    UnkValue_0x18 result;
    fn_71000a18d4(&result, &g_unk_7101cc2e90, data, true);
    return result;
}
