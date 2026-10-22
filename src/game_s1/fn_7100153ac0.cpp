// The static initialiser of a unit with a small polymorphic object (a vtable and a pointer) and
// five 0x130-byte objects that each end with such an object, destroyed by fn_7100153998.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

extern void* g_unk_7101a1d770[];

struct UnkPtrHolder {
    void** vtable;
    void* ptr;
};

struct UnkObject_0x130 {
    u8 field_0x0[0x100];
    u64 field_0x100;
    u64 field_0x108;
    u32 field_0x110;
    UnkPtrHolder field_0x118;
    u16 field_0x128;
};

__attribute__((always_inline)) inline void InitHolder(UnkPtrHolder* h) {
    h->vtable = &g_unk_7101a1d770[2];
    h->ptr = nullptr;
}

void fn_7100153998(void* object);

__attribute__((always_inline)) inline void InitObject(UnkObject_0x130* o) {
    o->field_0x100 = 0;
    o->field_0x108 = 0;
    o->field_0x110 = 0;
    InitHolder(&o->field_0x118);
    o->field_0x128 = 0;
    __cxa_atexit(fn_7100153998, o, g_unk_7101a19000);
}

extern HeaderIds2 g_unk_716e275790 __attribute__((visibility("hidden")));
extern UnkPtrHolder g_unk_716e275190;
extern UnkObject_0x130 g_unk_716e2751a0;
extern UnkObject_0x130 g_unk_716e2752d0;
extern UnkObject_0x130 g_unk_716e275400;
extern UnkObject_0x130 g_unk_716e275530;
extern UnkObject_0x130 g_unk_716e275660;

void fn_7100153ac0() {
    InitHeaderIds(&g_unk_716e275790);
    InitHolder(&g_unk_716e275190);
    InitObject(&g_unk_716e2751a0);
    InitObject(&g_unk_716e2752d0);
    InitObject(&g_unk_716e275400);
    InitObject(&g_unk_716e275530);
    InitObject(&g_unk_716e275660);
}
