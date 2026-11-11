#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRenderTargetA1;
struct UnkBoundTarget;

extern CStrId g_unk_716e596120 __attribute__((visibility("hidden")));
extern CStrId g_unk_716e596210 __attribute__((visibility("hidden")));

void fn_71001cdd6c(UnkRenderTargetA1* target, CStrId id);
void fn_71001cdd70(UnkRenderTargetA1* target);
void* fn_710015c450(UnkBoundTarget* target);

struct UnkSetupSubB {
    u8 pad_0x0[0x1];
};
struct UnkSetupArgB {
    u64 a;
    u32 b;
    u32 c;
};
void fn_710014d150(UnkSetupSubB* sub, void* block, u32 flag);
void fn_710014d17c(UnkSetupSubB* sub, UnkSetupArgB* arg);
void fn_710014cb98(UnkSetupSubB* sub, u32 a, void* id, u32 b, u32 c);
void fn_710014c864(UnkSetupSubB* sub, void* p, u32 a);
void fn_710014d1a0(UnkSetupSubB* sub);

struct UnkSetupOwnerB {
    u8 pad_0x0[0xf0];
    UnkRenderTargetA1* target;  // 0xf0
    u8 pad_0xf8[0x8];
    u8 block_0x100[0x18];
    u64 field_0x118;
    u32 field_0x120;
    u8 field_0x124[8];
    u32 field_0x12c;
    u8 pad_0x130[0x2e8];
    void* field_0x418;
    u64 field_0x420;
    u32 field_0x428;
    u8 field_0x42c[8];
    u32 field_0x434;
    u8 pad_0x438[4];
    u8 field_0x43c;
    u8 pad_0x43d[0x33];
    UnkBoundTarget* slots_0x470[3];
    u32 field_0x488;
    u32 field_0x48c;
    u8 pad_0x490[0x2450];
    UnkSetupSubB sub_0x28e0;
};
static_assert(__builtin_offsetof(UnkSetupOwnerB, sub_0x28e0) == 0x28e0, "UnkSetupOwnerB layout");
static_assert(__builtin_offsetof(UnkSetupOwnerB, field_0x488) == 0x488, "UnkSetupOwnerB layout");

__attribute__((noinline)) void fn_71001dba58(UnkSetupOwnerB* self, void* id, void* p) {
    fn_71001cdd6c(self->target, g_unk_716e596210);
    UnkSetupSubB* sub = &self->sub_0x28e0;
    fn_710014d150(sub, self->block_0x100, 1);
    u64 a = self->field_0x118;
    u32 b = self->field_0x120;
    u32 c = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&self->field_0x118) + 0xc);
    UnkSetupArgB arg;
    arg.a = a;
    arg.b = b;
    arg.c = c;
    fn_710014d17c(sub, &arg);
    fn_710014cb98(sub, 0, &id, 0, 0);
    fn_710014c864(sub, p, 0);
    fn_710014d1a0(sub);
    fn_71001cdd70(self->target);
}

void fn_71001db2b8(UnkSetupOwnerB* self) {
    fn_71001cdd6c(self->target, g_unk_716e596120);
    u32 index = self->field_0x48c;
    u32 saved = self->field_0x488;
    self->field_0x488 = index;
    fn_71001dba58(self, fn_710015c450(self->slots_0x470[index]), self->field_0x418);
    self->field_0x420 = self->field_0x118;
    u32 w = self->field_0x120;
    self->field_0x488 = saved;
    u8* dst = self->field_0x42c;
    self->field_0x428 = w;
    __builtin_memcpy(dst, reinterpret_cast<u8*>(&self->field_0x120) + 4, 8);
    self->field_0x434 = self->field_0x12c;
    fn_71001cdd70(self->target);
    self->field_0x43c = 0;
}
