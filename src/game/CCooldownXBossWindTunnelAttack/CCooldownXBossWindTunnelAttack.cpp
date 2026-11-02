// Two identical constructors in the CCooldownXBossWindTunnelAttack unit (fn_71004cb480,
// fn_71004cc988): the base constructor fn_71004c1894, the vtable, then cleared fields from +0x68 to
// +0x90. Names and layouts are placeholders.
#include "types.h"

struct UnkNode;
void fn_71004c1894(UnkNode* self);
extern void* g_unk_7101a3eae0[];

struct UnkWindTunnelNode {
    void** vtable;
    u8 pad_0x8[0x60];
    u64 field_0x68;
    u64 field_0x70;
    u8 field_0x78;
    u8 pad_0x79[3];
    u32 field_0x7c;
    u32 field_0x80;
    u8 field_0x84;
    u8 pad_0x85[3];
    u32 field_0x88;
    u8 field_0x8c;
    u8 pad_0x8d[3];
    u32 field_0x90;
};

__attribute__((always_inline)) static inline void ConstructWindTunnelNode(UnkWindTunnelNode* self) {
    fn_71004c1894(reinterpret_cast<UnkNode*>(self));
    self->vtable = g_unk_7101a3eae0 + 2;
    self->field_0x88 = 0;
    self->field_0x8c = 0;
    self->field_0x90 = 0;
    self->field_0x68 = 0;
    self->field_0x70 = 0;
    self->field_0x78 = 0;
    self->field_0x7c = 0;
    self->field_0x80 = 0;
    self->field_0x84 = 0;
}

void fn_71004cb480(UnkWindTunnelNode* self) {
    ConstructWindTunnelNode(self);
}

void fn_71004cc988(UnkWindTunnelNode* self) {
    ConstructWindTunnelNode(self);
}
