// Constructor of an object derived from fn_7100393e68's: sets its vtable, clears its members and sets
// a few flags and sentinel values (-1 for an index and a float pair). The same initialisation as
// fn_710037ca84, without the call that one finishes in.
#include "types.h"

typedef u64 u64_a4 __attribute__((aligned(4)));

struct UnkNode {
    void** vtable;
};

void fn_7100393e68(UnkNode*);
extern void* g_unk_7101a25a60[];

template <class T>
static inline void St(UnkNode* self, u32 off, T value) {
    *reinterpret_cast<T*>(reinterpret_cast<char*>(self) + off) = value;
}

void fn_710037a410(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a25a60 + 2;
    St<u64_a4>(self, 0x2c, 0);
    St<u64_a4>(self, 0x34, 0);
    St<u64_a4>(self, 0x3c, 0);
    St<u64_a4>(self, 0x42, 0);
    St<u8>(self, 0x4a, 1);
    St<u32>(self, 0x4c, 0xbf800000u);
    St<u16>(self, 0x50, 0);
    St<u8>(self, 0x52, 1);
    __builtin_memset(reinterpret_cast<char*>(self) + 0x5c, 0, 12);
    __builtin_memset(reinterpret_cast<char*>(self) + 0x54, 0, 7);
    St<u64>(self, 0x68, 0xbf800000bf800000ull);
    St<u8>(self, 0x70, 0);
    St<u64>(self, 0x78, 0);
    St<u64>(self, 0x80, 0);
    St<u32>(self, 0x88, 0xffffffffu);
    St<u64_a4>(self, 0x8c, 0);
    St<u64_a4>(self, 0x94, 0);
    St<u64_a4>(self, 0x9c, 0);
    St<u64_a4>(self, 0xa4, 0);
    St<u64_a4>(self, 0xac, 0);
    St<u64_a4>(self, 0xb4, 0);
    St<u64>(self, 0xc0, 0);
    St<u64>(self, 0xc8, 0xbf80000000000000ull);
    St<u64>(self, 0xd0, 0);
    St<u32>(self, 0xd8, 0);
}
