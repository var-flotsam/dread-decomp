// Node routine that calls fn_71004c1894, fn_7100d62b04, sets its vtable and initialises or clears fields.
#include <new>

#include "base/global/CStrId.h"
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStrId;

template <class T>
struct __attribute__((packed)) UnkPacked {
    T v;
};

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x60];
    u64 field_0x68;
    void** field_0x70;
    u64 field_0x78;
    u64 field_0x80;
    u64 field_0x88;
    u8 pad_0x90[0x130];
    u64 field_0x1c0;
    u8 pad_0x1c8[0x8];
    u64 field_0x1d0;
    u32 field_0x1d8;
    u8 pad_0x1dc[0x5a0];
    UnkPacked<u64> field_0x77c;
    u16 field_0x784;
    u8 field_0x786;
};

void fn_71004c1894(UnkNode*);
extern void* g_unk_7101a3e5b8[];
extern void* g_unk_7101a22d48[];
void fn_7100d62b04(void*);

void fn_71004c5738(UnkNode* self) {
    fn_71004c1894(self);
    self->vtable = &g_unk_7101a3e5b8[2];
    self->field_0x70 = &g_unk_7101a22d48[2];
    self->field_0x68 = 0;
    self->field_0x78 = 0;
    self->field_0x80 = 0;
    self->field_0x88 = 0;
    fn_7100d62b04(reinterpret_cast<char*>(self) + 0xa0);
    self->field_0x1c0 = 0xffffffff00000000ul;
    self->field_0x1d0 = 0;
    self->field_0x1d8 = 0;
    self->field_0x77c.v = 0;
    self->field_0x784 = 1;
    self->field_0x786 = 0;
}
