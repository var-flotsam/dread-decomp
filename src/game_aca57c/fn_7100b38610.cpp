// Constructor of a class in the family whose destructors are in fn_7100b395fc.cpp: the base
// (vtable 0x7101a9c8e0) holds flags, an empty string at 0x28, three floats of 1, a string with
// 0x20 bytes of inline storage at 0x70 and a few words; the class (vtable 0x7101a9c888) adds a
// word and the empty string id.
#include <cstddef>
#include <new>

#include "UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;

extern void* g_unk_7101a9c8e0[];
extern void* g_unk_7101a9c888[];
extern void* g_unk_7101a1b5a0[];
using UnkString20 = UnkBufferString<0x20, g_unk_7101a1b5a0>;

__attribute__((always_inline)) static inline void SetVtable(void* self, void** vtable) {
    *reinterpret_cast<void***>(self) = vtable + 2;
}

struct UnkNamedBase {
    void** vtable;
    bool field_0x8;
    f32 field_0xc[3];
    f32 field_0x18[3];
    CRntString name;
    u32 field_0x58;
    alignas(8) f32 field_0x60[3];
    UnkString20 holder;
    u32 field_0xd8;
    alignas(8) u32 field_0xe0;
    u16 field_0xe4;
    u32 field_0xe8;
    bool field_0xec;

    __attribute__((always_inline)) UnkNamedBase()
        : field_0x8((SetVtable(this, g_unk_7101a9c8e0), true)), field_0xc{}, field_0x18{}, name(""), field_0x58(0),
          field_0x60{1.0f, 1.0f, 1.0f}, field_0xd8(0), field_0xe0(0), field_0xe4(1), field_0xe8(0),
          field_0xec(true) {}
};
static_assert(offsetof(UnkNamedBase, field_0xd8) == 0xd8, "UnkNamedBase layout");

// The empty string id, set directly (the pool is created first if there is none yet).
struct UnkDirectEmptyId {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkDirectEmptyId() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

struct UnkNamedWithEmptyId : UnkNamedBase {
    u64 field_0xf0;
    UnkDirectEmptyId id;
    u64 field_0x100;

    __attribute__((always_inline)) UnkNamedWithEmptyId()
        : field_0xf0((SetVtable(this, g_unk_7101a9c888), 0)), field_0x100(0) {}
};
static_assert(sizeof(UnkNamedWithEmptyId) == 0x108, "UnkNamedWithEmptyId layout");

void fn_7100b38610(UnkNamedWithEmptyId* self) { new (self) UnkNamedWithEmptyId(); }

// The class with vtable 0x7101a9ca40: a word, the empty string id, a word and a flag.
extern void* g_unk_7101a9ca40[];

struct UnkNamedWithIdFlag : UnkNamedBase {
    u32 field_0xf0;
    UnkDirectEmptyId id;
    u32 field_0x100;
    bool field_0x104;

    __attribute__((always_inline)) UnkNamedWithIdFlag()
        : field_0xf0((SetVtable(this, g_unk_7101a9ca40), 0)), field_0x100(0), field_0x104(false) {}
};

void fn_7100b3874c(UnkNamedWithIdFlag* self) { new (self) UnkNamedWithIdFlag(); }
