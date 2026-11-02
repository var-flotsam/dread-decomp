// The constructor of behaviortree::CParallel (fn_71003a8854): the base constructor fn_710039618c,
// the vtable, the policy word at +0x60 (1), the array at +0x68 (fn_71003a7c9c: capacity 10,
// growth 4), two cleared fields, then the post-construction virtual call (slot 3). Names and
// layouts are placeholders.
#include "types.h"

void fn_710039618c(void* self);
void fn_71003a7c9c(void* self, u32 capacity, u32 field_2, u32 growth, void* field_4);
extern void* g_unk_7101a29148[];

struct UnkParallelNode {
    void** vtable;
    u8 pad_0x8[0x50];
    u64 field_0x58;
    u32 field_0x60;
    u8 pad_0x64[4];
    u8 field_0x68[0x70];
    u64 field_0xd8;
    u32 field_0xe0;
};

void fn_71003a8854(UnkParallelNode* self) {
    fn_710039618c(self);
    self->field_0x58 = 0;
    self->vtable = g_unk_7101a29148 + 2;
    self->field_0x60 = 1;
    fn_71003a7c9c(self->field_0x68, 10, 0, 4, nullptr);
    self->field_0xd8 = 0;
    self->field_0xe0 = 0;
    reinterpret_cast<void (*)(UnkParallelNode*)>(self->vtable[3])(self);
}
