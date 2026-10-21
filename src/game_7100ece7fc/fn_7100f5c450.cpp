// Constructors of a family of small objects sharing one base (vtable g_unk_7101cab1f8, an empty
// string id and a zeroed 8-byte field): the base's, then the class's vtable and its own fields.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

extern void* g_unk_7101cab1f8[];

struct UnkNoTail {};

// The base, followed by `Tail` (zeroed with the base's own fields).
template <class Tail>
struct UnkBase {
    void** vtable;
    UnkStringData* id;  // a CStrId
    f32 field_0x10;
    u32 field_0x14;
    Tail tail;

    __attribute__((always_inline)) UnkBase() {
        field_0x10 = 0;
        field_0x14 = 0;
        tail = Tail();
        vtable = &g_unk_7101cab1f8[2];
        id = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        id = &g_unk_7101d47108->empty;
    }
};

using UnkBase_0x18 = UnkBase<UnkNoTail>;

struct UnkTail_0x8 {
    u32 field_0x18;
    u32 field_0x1c;
};

struct UnkTail_0x10 {
    u64 field_0x18;
    u64 field_0x20;
};

template <void** Vtable>
struct UnkObject : UnkBase_0x18 {
    __attribute__((always_inline)) UnkObject() { vtable = &Vtable[2]; }
};

// The same with 1.0 in the base's first field.
template <void** Vtable>
struct UnkObjectOne : UnkBase_0x18 {
    __attribute__((always_inline)) UnkObjectOne() {
        vtable = &Vtable[2];
        field_0x10 = 1.0f;
    }
};

template <void** Vtable, class Tail>
struct UnkObjectTail : UnkBase<Tail> {
    __attribute__((always_inline)) UnkObjectTail() { this->vtable = &Vtable[2]; }
};

template <void** Vtable>
using UnkObject8 = UnkObjectTail<Vtable, UnkTail_0x8>;

template <void** Vtable>
using UnkObject10 = UnkObjectTail<Vtable, UnkTail_0x10>;

// The same, setting the u32 at 0x1c again.
template <void** Vtable>
struct UnkObject8B : UnkBase<UnkTail_0x8> {
    __attribute__((always_inline)) UnkObject8B() {
        vtable = &Vtable[2];
        tail.field_0x1c = 0;
    }
};

#define DEFINE_CTOR(addr, Type, vt)                  \
    extern void* g_unk_##vt[];                       \
    void fn_##addr(Type<g_unk_##vt>* self) {         \
        new (self) Type<g_unk_##vt>;                 \
    }

DEFINE_CTOR(7100f5c450, UnkObject8, 7101cab220)
DEFINE_CTOR(7100f6ce5c, UnkObject10, 7101cab338)
DEFINE_CTOR(7100f838d0, UnkObject8B, 7101cab400)
DEFINE_CTOR(7100f5e9b4, UnkObject, 7101cab248)
DEFINE_CTOR(7100f61090, UnkObject, 7101cab270)
DEFINE_CTOR(7100f63380, UnkObject, 7101cab298)
DEFINE_CTOR(7100f65abc, UnkObject, 7101cab2c0)
DEFINE_CTOR(7100f67f3c, UnkObject, 7101cab2e8)
DEFINE_CTOR(7100f6a158, UnkObject, 7101cab310)
DEFINE_CTOR(7100f6fa44, UnkObject, 7101cab360)
DEFINE_CTOR(7100f71ba8, UnkObjectOne, 7101cab388)
DEFINE_CTOR(7100f74368, UnkObjectOne, 7101cab3b0)
DEFINE_CTOR(7100f76588, UnkObjectOne, 7101cab3d8)
