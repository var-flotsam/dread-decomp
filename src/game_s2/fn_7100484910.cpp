// Resets the owner's objects: runs two calls on the one at +0x2a0, then looks up three components of
// the one at +0x280 by hash (keeping those the kind checks accept) and resets each; finally, when the
// handle at +0x98 is still current (its serial at +0x8 equals the one at +0xa0, which is not -1),
// calls fn_7100105a80 on its target with 0.2 and 0.
#include "types.h"

struct UnkComponent {
    u8 pad_0x0[0xb];
    u8 field_0xb;
};

struct UnkObject;

struct UnkOwner {
    u8 pad_0x0[0x280];
    UnkObject* field_0x280;
    u8 pad_0x288[0x18];
    UnkObject* field_0x2a0;
};

struct UnkHandle {
    void* field_0x0;
    u64 field_0x8;
};

struct UnkSelf {
    u8 pad_0x0[8];
    UnkOwner* field_0x8;
    u8 pad_0x10[0x88];
    UnkHandle* field_0x98;
    u64 field_0xa0;
};

extern const u64 g_unk_71726cf1e0[] __attribute__((visibility("hidden")));
void fn_7100863a8c(UnkObject* object, const u64* key);
UnkComponent* fn_7101152b90(UnkObject* object, u64 hash);
bool fn_710114e680(UnkObject* object, u8 kind);
bool fn_710114e668(UnkObject* object, u8 kind);
void fn_7100145e94(UnkComponent* component, u32 flag);
void fn_7100146e60(UnkComponent* component, u32 flag);
void fn_7100105a80(void* target, f32 a, f32 b);

void fn_7100484910(UnkSelf* self) {
    if (UnkObject* object = self->field_0x8->field_0x2a0) {
        fn_7100863a8c(object, g_unk_71726cf1e0);
        fn_7100863a8c(object, g_unk_71726cf1e0 + 1);
    }
    if (UnkObject* object = self->field_0x8->field_0x280) {
        UnkComponent* a = fn_7101152b90(object, 0x8db4647a121a9a23ull);
        if (a)
            a = fn_710114e680(object, a->field_0xb) ? a : nullptr;
        fn_7100145e94(a, 0);
        UnkComponent* b = fn_7101152b90(object, 0x8f81a6f5f469556bull);
        if (b)
            b = fn_710114e680(object, b->field_0xb) ? b : nullptr;
        fn_7100145e94(b, 0);
        UnkComponent* c = fn_7101152b90(object, 0x99b1c49dad0a0ac2ull);
        if (c)
            c = fn_710114e668(object, c->field_0xb) ? c : nullptr;
        fn_7100146e60(c, 0);
    }
    UnkHandle* handle = self->field_0x98;
    if (handle && handle->field_0x8 == self->field_0xa0 && self->field_0xa0 != ~0ull && handle->field_0x0)
        fn_7100105a80(handle->field_0x0, 0.2f, 0.0f);
}
