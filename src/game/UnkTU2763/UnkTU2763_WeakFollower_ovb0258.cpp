#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101cb7130[];

struct UnkWeakFollower_b0258q {
    void** vtable;  // 0x0
    bool field_0x8;
    u8 pad_0x9[3];
    u32 field_0xc[2];
    u32 field_0x14[2];
    f32 field_0x1c[2];
    u32 field_0x24[2];
    u32 field_0x2c[2];
    u32 field_0x34;
    UnkWeakHandle* handle;  // 0x38
} __attribute__((packed));
static_assert(__builtin_offsetof(UnkWeakFollower_b0258q, handle) == 0x38, "layout");

void fn_710120280c(UnkWeakFollower_b0258q* self, UnkWeakTarget* target) {
    self->field_0x8 = false;
    self->vtable = &g_unk_7101cb7130[2];
    self->field_0xc[0] = 0;
    self->field_0xc[1] = 0;
    self->field_0x14[0] = 0;
    self->field_0x14[1] = 0;
    self->field_0x24[0] = 0;
    self->field_0x24[1] = 0;
    self->field_0x2c[0] = 0;
    self->field_0x2c[1] = 0;
    self->field_0x34 = 0;
    self->field_0x1c[0] = -1.0f;
    self->field_0x1c[1] = -1.0f;
    UnkWeakHandle* handle = GetWeakHandle(target);
    self->handle = nullptr;
    AssignRef(&self->handle, handle);
}
