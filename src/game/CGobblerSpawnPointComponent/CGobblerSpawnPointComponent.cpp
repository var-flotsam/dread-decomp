// CGobblerSpawnPointComponent: its constructor (fn_7100a1a8cc). The spawn point base constructor
// fn_71011258e4, the vtable, two callback lists at +0x3b8 and +0x3f8 (base vtable with an empty array
// of callables: step 10, flags 0xf; then the list's vtable and cleared fields), the flag at +0x438,
// then the class-entry pass of base::core::CBaseObject (fn_7100023854). Names and layouts are
// placeholders.
//
// The fields are addressed by offset: through a struct, clang forms the list's address once and
// stores its flag through it, unlike the original.
#include "base/UnkVector.h"
#include "base/core/CBaseObject.h"
#include "game/UnkTU2704.h"

struct UnkNode;

extern char g_unk_7101a8c8d8[];  // the component's vtable
extern void* g_unk_7101a20998[];  // the callback list base's vtable
extern void* g_unk_7101a33fa0[];  // the callback list's vtable

template <class T>
static inline T& At(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

// A callback list at `o`: the base's vtable and empty array (data, count and capacity, step, a flag,
// allocator flags, allocator), a cleared word, the list's vtable and a cleared pointer.
static inline __attribute__((always_inline)) void InitList(void* self, u64 o) {
    At<char*>(self, o) = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
    At<u64>(self, o + 8) = 0;
    At<u64>(self, o + 0x10) = 0;
    At<u32>(self, o + 0x18) = 10;
    At<u8>(self, o + 0x1c) = 1;
    At<u32>(self, o + 0x20) = 0xf;
    At<base::UnkAllocator*>(self, o + 0x28) = fn_7100083990();
    At<u32>(self, o + 0x30) = 0;
    At<char*>(self, o) = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
    At<u64>(self, o + 0x38) = 0;
}

void fn_7100a1a8cc(void* self) {
    fn_71011258e4(self);
    At<char*>(self, 0) = g_unk_7101a8c8d8 + 0x10;
    InitList(self, 0x3b8);
    InitList(self, 0x3f8);
    At<bool>(self, 0x438) = false;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
