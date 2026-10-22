// Computes a hash over an object's state: opens a stream (kind 0x16), appends its id, a
// 64-bit value reached through a pointer chain, and returns the digest of the buffer.
#include "types.h"

struct UnkStream_0x38 {
    u8 pad_0x0[0x18];
    void* dest_0x18;
    u8 pad_0x20[0x18];
};

void fn_710007be8c(UnkStream_0x38* s, s32 kind, void* arg);
void fn_710007c3f0(UnkStream_0x38* s, const void* data, u32 size);
void* fn_710007c1a4(UnkStream_0x38* s);
void fn_710007c0e4(UnkStream_0x38* s);
s32 fn_710007b750(void* dest, void* buffer, s32 count);

struct UnkScopedStream : UnkStream_0x38 {
    UnkScopedStream(s32 kind, void* arg) { fn_710007be8c(this, kind, arg); }
    ~UnkScopedStream() { fn_710007c0e4(this); }
    template <class T>
    void Put(T v) {
        fn_710007c3f0(this, &v, sizeof(T));
    }
};

struct UnkInner {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkNode {
    u8 pad_0x0[8];
    UnkInner* inner_0x8;
};

struct UnkHashed {
    u8 pad_0x0[0xe0];
    u32 field_0xe0;
    u8 pad_0xe4[4];
    UnkNode* node_0xe8;
};

u32 fn_710009d110(UnkHashed* self);

s32 fn_710009a394(UnkHashed* self) {
    UnkScopedStream s(0x16, nullptr);
    s.Put(fn_710009d110(self));
    s.Put(self->node_0xe8->inner_0x8->field_0x30);
    return fn_710007b750(s.dest_0x18, fn_710007c1a4(&s), -1);
}
