// A constructor in the CAnimationBlendSkeletonTransformer unit, emitted twice (complete and base
// object, byte-identical): the base part (fn_7100005a88), the vtable, two members built by
// fn_7100014d94, then a block of plain fields. Names and layouts are placeholders.
#include "types.h"

void fn_7100005a88(void* self);
void fn_7100014d94(void* self, u32 arg);

extern char g_unk_7101a19018[];

struct UnkBlendTransformer {
    char* vtable;
    u8 base[0x58];
    u8 member_0x60[0x38];
    u8 member_0x98[0x38];
    u64 field_0xd0;
    u64 field_0xd8;
    u64 field_0xe0;
    u64 field_0xe8;
    u64 field_0xf0;
    u64 field_0xf8;
    u8 field_0x100;
};

__attribute__((always_inline)) static inline void Construct(UnkBlendTransformer* self) {
    fn_7100005a88(self);
    self->vtable = g_unk_7101a19018 + 0x10;
    fn_7100014d94(self->member_0x60, 0);
    fn_7100014d94(self->member_0x98, 0);
    self->field_0x100 = 0;
    self->field_0xd0 = 0;
    self->field_0xd8 = 0;
    self->field_0xe0 = 0x3f80000000000000;  // 0.0f, 1.0f
    self->field_0xe8 = 0x7fffffffbf800000;  // -1.0f, INT32_MAX
    self->field_0xf0 = 0;
    self->field_0xf8 = 0;
}

void fn_710000379c(UnkBlendTransformer* self) { Construct(self); }
void fn_7100003e4c(UnkBlendTransformer* self) { Construct(self); }
