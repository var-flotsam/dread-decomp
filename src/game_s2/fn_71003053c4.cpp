// Resets the three special-burst rocket ball effects on the object at +0x280 of the owner's owner
// (looked up by the hash of their names), then, when the handle at +0x400 is still current (its
// serial at +0x8 equals the one at +0x408, which is not -1), runs fn_7100072328 on its target with
// 0.2 and 0 and clears the handle (copies the null handle).
#include "types.h"

struct UnkHandleTarget {
    void* field_0x0;
    u64 field_0x8;
};

struct UnkObject;

struct UnkOwnerObject {
    u8 pad_0x0[0x280];
    UnkObject* field_0x280;
};

struct UnkOwnerRef {
    u8 pad_0x0[0x30];
    UnkOwnerObject* field_0x30;
};

struct UnkHandle {
    UnkHandleTarget* target;
    u64 serial;
};

struct UnkSelf {
    u8 pad_0x0[0xc0];
    UnkOwnerRef* field_0xc0;
    u8 pad_0xc8[0x338];
    UnkHandle field_0x400;
};

extern UnkHandle g_unk_7101d586c0;
u64 fn_7100001570(const char* str, u64 length);
void fn_710114fe8c(UnkObject* object, u64 hash, u32 a, u32 b);
void fn_7100072328(void* target, f32 a, f32 b);

void fn_71003053c4(UnkSelf* self) {
    fn_710114fe8c(self->field_0xc0->field_0x30->field_0x280, fn_7100001570("specialburst_rocketball", 23), 0, 0);
    fn_710114fe8c(self->field_0xc0->field_0x30->field_0x280, fn_7100001570("specialburst_rocketballcenter", 29), 0,
                  0);
    fn_710114fe8c(self->field_0xc0->field_0x30->field_0x280, fn_7100001570("specialburst_rocketballcharging", 31), 0,
                  0);
    UnkHandle* handle = &self->field_0x400;
    if (handle->target && handle->target->field_0x8 == handle->serial && handle->serial != ~0ull &&
        handle->target->field_0x0) {
        fn_7100072328(handle->target->field_0x0, 0.2f, 0.0f);
        *handle = g_unk_7101d586c0;
    }
}
