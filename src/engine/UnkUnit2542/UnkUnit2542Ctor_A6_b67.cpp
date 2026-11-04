// Unit 2542 (tools/tu_map.py): constructor of the 0x60-byte startup singleton src/engine/UnkStartupManagers.cpp
// creates (g_unk_7172758b18). Two interface bases (vtables at +0 and +8) and a member at +0x10 built by
// fn_7100db2fb4(member, 0, 0xe); the class's own vtable group then replaces both. Names and layout are
// placeholders.
#include "types.h"

extern void* g_unk_7101aa93c8[];
extern void* g_unk_7101aa93e8[];
extern void* g_unk_7101aa9348[];

void fn_7100db2fb4(void* self, void* a, u32 b);

struct UnkUnit2542_A6_b67 {
    void* vtable_0x0;
    void* vtable_0x8;
    u8 field_0x10[0x50];
};

void fn_7100db23bc(void* p) {
    UnkUnit2542_A6_b67* self = static_cast<UnkUnit2542_A6_b67*>(p);
    self->vtable_0x0 = reinterpret_cast<char*>(g_unk_7101aa93c8) + 0x10;
    self->vtable_0x8 = reinterpret_cast<char*>(g_unk_7101aa93e8) + 0x10;
    fn_7100db2fb4(self->field_0x10, nullptr, 0xe);
    self->vtable_0x0 = reinterpret_cast<char*>(g_unk_7101aa9348) + 0x10;
    self->vtable_0x8 = reinterpret_cast<char*>(g_unk_7101aa9348) + 0x30;
}
