// Constructor of a component: runs the base constructor, then builds three empty arrays (flags 0xf),
// a 7-word record, and two embedded objects (each sets its own vtable, then the final one's) before
// finishing with the vtable's slot at +0x18 on itself.
//
// The fields are addressed by offset: the original's stores are scheduled by the constant pool of
// the whole function, which struct member stores don't reproduce.
#include "types.h"

extern void* g_unk_7101a337d8[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];

// Only the vtable at +0x0 is named; the size 1 keeps offsets from folding into element strides.
struct UnkComponent {
    u8 unk_0x0;
};

void fn_710036932c(UnkComponent* self);
void* fn_7100083990_alloc();
void fn_710035d4e0(void* member);

struct __attribute__((packed)) UnkRecord {
    u64 words[6];
    u64 limit;
};

template <class T>
static inline T& At(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

void fn_71004076e4(UnkComponent* self) {
    fn_710036932c(self);
    At<void**>(self, 0) = g_unk_7101a337d8 + 2;
    At<u64>(self, 0x146d0) = 0;
    At<u64>(self, 0x146d8) = 0;
    At<u32>(self, 0x146e0) = 10;
    At<u8>(self, 0x146e4) = 1;
    At<u32>(self, 0x146e8) = 0xf;
    At<void*>(self, 0x146f0) = fn_7100083990_alloc();
    At<u8>(self, 0x146f8) = 0;
    UnkRecord* record = &At<UnkRecord>(self, 0x146fc);
    __builtin_memset(record->words, 0, sizeof(record->words));
    record->limit = 0x7fffffff00000000ull;
    fn_710035d4e0(reinterpret_cast<char*>(self) + 0x14738);
    void** vtable_a = g_unk_7101a20998 + 2;
    At<void**>(self, 0x14750) = vtable_a;
    At<u64>(self, 0x14758) = 0;
    At<u64>(self, 0x14760) = 0;
    At<u32>(self, 0x14768) = 10;
    At<u8>(self, 0x1476c) = 1;
    At<u32>(self, 0x14770) = 0xf;
    void* alloc_b = fn_7100083990_alloc();
    void** vtable_b = g_unk_7101a33fa0 + 2;
    At<void**>(self, 0x14750) = vtable_b;
    At<void*>(self, 0x14778) = alloc_b;
    At<u32>(self, 0x14780) = 0;
    At<u64>(self, 0x14788) = 0;
    At<void**>(self, 0x14790) = vtable_a;
    At<u64>(self, 0x14798) = 0;
    At<u64>(self, 0x147a0) = 0;
    At<u32>(self, 0x147a8) = 10;
    At<u8>(self, 0x147ac) = 1;
    At<u32>(self, 0x147b0) = 0xf;
    void* alloc_c = fn_7100083990_alloc();
    At<void*>(self, 0x147b8) = alloc_c;
    At<u32>(self, 0x147c0) = 0;
    At<void**>(self, 0x14790) = vtable_b;
    At<u64>(self, 0x147c8) = 0;
    At<u8>(self, 0x147d0) = 0;
    reinterpret_cast<void (*)(UnkComponent*)>(At<void**>(self, 0)[3])(self);
}
