// Constructor of a component: runs the base constructor, then sets its defaults (ids of -1,
// embedded objects with their vtables, empty records, a count of 0x13 and an allocator) and finishes
// with the vtable's slot at +0x18 on itself.
//
// The fields are addressed by offset: the original's stores are scheduled by the constant pool of
// the whole function, which struct member stores don't reproduce.
#include "types.h"

extern void* g_unk_7101a47e90[];
extern void* g_unk_7101a22d48[];
extern void* g_unk_7101a47f48[];
extern void* g_unk_7101a47f88[];

// Only the vtable at +0x0 is named; the size 1 keeps offsets from folding into element strides.
struct UnkComponent {
    u8 unk_0x0;
};

void fn_710051dab0(UnkComponent* self);
void fn_71002acfe4(void* member);
void* fn_7100083990_alloc();

template <class T>
static inline T& At(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

void fn_710051d360(UnkComponent* self) {
    fn_710051dab0(self);
    At<void**>(self, 0) = g_unk_7101a47e90 + 2;
    At<u64>(self, 0x30) = ~0ull;
    At<u64>(self, 0x38) = ~0ull;
    At<void**>(self, 0x40) = g_unk_7101a22d48 + 2;
    At<u64>(self, 0x48) = 0;
    At<u64>(self, 0x50) = 0;
    At<u64>(self, 0x58) = 0;
    At<u64>(self, 0x5d) = 0;
    fn_71002acfe4(reinterpret_cast<char*>(self) + 0x68);
    At<u16>(self, 0x12af0) = 0;
    At<u8>(self, 0x12af2) = 0;
    At<void**>(self, 0x12af8) = g_unk_7101a47f48 + 2;
    At<void**>(self, 0x12b00) = g_unk_7101a47f88 + 2;
    At<u32>(self, 0x12b08) = 0;
    At<u64>(self, 0x12b10) = 0;
    At<u64>(self, 0x12b18) = 0;
    void* alloc = fn_7100083990_alloc();
    At<u32>(self, 0x12b28) = 0x13;
    At<u16>(self, 0x12b34) = 1;
    At<void*>(self, 0x12b20) = alloc;
    At<u32>(self, 0x12b30) = 0;
    At<u8>(self, 0x12b36) = 0;
    At<u64>(self, 0x12b38) = ~0ull;
    At<u64>(self, 0x12b40) = 0;
    At<u64>(self, 0x12b48) = 0;
    reinterpret_cast<void (*)(UnkComponent*)>(At<void**>(self, 0)[3])(self);
}
