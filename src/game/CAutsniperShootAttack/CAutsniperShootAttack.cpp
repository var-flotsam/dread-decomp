// Two identical constructors in the CAutsniperShootAttack unit (fn_71003d377c, fn_71003d84a8): the
// base constructor fn_710036069c, the vtable, cleared words, a member at +0x28 whose vtable is set
// at +0x30, and the sub-object at +0xd0 (fn_71002acfe4). Names and layouts are placeholders.
#include "types.h"

void fn_710036069c(void* self);
void fn_71002acfe4(void* sub);
extern void* g_unk_7101a2e5d0[];
extern void* g_unk_7101a2aef8[];

struct UnkAutsniperNode {
    void** vtable;
    u8 pad_0x8[0x20];
    u64 field_0x28;
    void** field_0x30;
    u64 field_0x38[3];
    u8 pad_0x50[0x8];
    u64 field_0x58[6];
    u8 pad_0x88[0x30];
    u64 field_0xb8[2];
    u8 pad_0xc8[0x8];
    u8 field_0xd0[0x10];
};

__attribute__((always_inline)) static inline void ConstructAutsniperNode(UnkAutsniperNode* self) {
    fn_710036069c(self);
    self->vtable = g_unk_7101a2e5d0 + 2;
    self->field_0x28 = 0;
    self->field_0x30 = g_unk_7101a2aef8 + 2;
    __builtin_memset(self->field_0x38, 0, sizeof(self->field_0x38));
    __builtin_memset(self->field_0xb8, 0, sizeof(self->field_0xb8));
    __builtin_memset(self->field_0x58, 0, sizeof(self->field_0x58));
    fn_71002acfe4(self->field_0xd0);
}

void fn_71003d377c(UnkAutsniperNode* self) {
    ConstructAutsniperNode(self);
}

void fn_71003d84a8(UnkAutsniperNode* self) {
    ConstructAutsniperNode(self);
}
