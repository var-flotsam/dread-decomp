// Constructor of a component: runs the base constructor, then sets its defaults (a range of 300.0,
// a few flags, empty arrays and records) and constructs three members before finishing with the
// vtable's slot at +0x18 on itself.
//
// The fields are addressed by offset.
#include "types.h"

extern void* g_unk_7101a3cbc8[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a20950[];
extern void* g_unk_7101a22d48[];

// Only the vtable at +0x0 is named; the rest is addressed by offset.
struct UnkComponent {
    u8 unk_0x0;
};

void fn_71003ac064(UnkComponent* self);
void* fn_7100083990_alloc();
void fn_7100494c00(void* member);
void fn_7100495430(void* member);
void fn_7100494158(void* member);

template <class T>
static inline T& At(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

void fn_710049d418(UnkComponent* self) {
    fn_71003ac064(self);
    At<void**>(self, 0) = g_unk_7101a3cbc8 + 2;
    At<f32>(self, 0x1499c) = 300.0f;
    At<u16>(self, 0x149a0) = 0x101;
    At<u16>(self, 0x14999) = 1;
    At<u64>(self, 0x149a4) = 0;
    At<u64>(self, 0x149ac) = 0;
    At<u64>(self, 0x149b4) = 0;
    At<u64>(self, 0x149bc) = 0;
    At<u32>(self, 0x149c4) = 0;
    At<u8>(self, 0x149c8) = 1;
    At<u32>(self, 0x149cc) = 0;
    At<void**>(self, 0x149d8) = g_unk_7101a20998 + 2;
    At<u64>(self, 0x149e0) = 0;
    At<u64>(self, 0x149e8) = 0;
    At<u32>(self, 0x149f0) = 10;
    At<u8>(self, 0x149f4) = 1;
    At<u32>(self, 0x149f8) = 0xf;
    At<void*>(self, 0x14a00) = fn_7100083990_alloc();
    At<u32>(self, 0x14a08) = 0;
    At<u64>(self, 0x14a10) = 0;
    At<u16>(self, 0x14a18) = 0;
    At<void**>(self, 0x149d8) = g_unk_7101a20950 + 2;
    At<u32>(self, 0x14a2c) = 0;
    At<u64>(self, 0x14a1c) = 0;
    At<u64>(self, 0x14a24) = 0;
    fn_7100494c00(reinterpret_cast<char*>(self) + 0x14a30);
    fn_7100495430(reinterpret_cast<char*>(self) + 0x14a78);
    fn_7100494158(reinterpret_cast<char*>(self) + 0x14ae0);
    At<u64>(self, 0x14b30) = 0;
    At<u32>(self, 0x14b38) = 0;
    At<u8>(self, 0x14b3c) = 0;
    At<u64>(self, 0x14b40) = ~0ull;
    At<u32>(self, 0x14b48) = 0;
    At<void**>(self, 0x14b50) = g_unk_7101a22d48 + 2;
    At<u64>(self, 0x14b68) = 0;
    At<u64>(self, 0x14b58) = 0;
    At<u64>(self, 0x14b60) = 0;
    reinterpret_cast<void (*)(UnkComponent*)>(At<void**>(self, 0)[3])(self);
}
