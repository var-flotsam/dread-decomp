// Builds a typed value from an object's payload, using a global type descriptor.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

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

extern const CType g_unk_7101cc2e90;

// NON_MATCHING(minor): one temporary lands in w8 instead of w9 (register allocation only).
UnkReflValue fn_710011ec5c(void* self, const UnkHolder* holder) {
    void* data = &holder->payload->data;
    if (holder->payload->kind == 2)
        data = *static_cast<void**>(data);
    UnkReflValue result;
    result.field_0x0 = nullptr;
    result.type = nullptr;
    result.kind = 2;
    fn_71000a18d4(&result, &g_unk_7101cc2e90, data, true);
    return result;
}
