// Unit 275 of tools/tu_map.py (0x71001f4dcc-0x71001f84ec, no reflected class): the destructor of an
// object that owns a helper (destroyed by fn_71001be078, then freed) and an array, over a base with a
// string id at 0x10, a counted resource at 0x78 and a member at 0x80 (destroyed by fn_7100216934); the
// base marks the word at 0x100 with 0xdeaddead. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "game/UnkTU275.h"
#include "types.h"

extern void* g_unk_7101a1f000[];

void fn_71001be078(char*);

struct UnkMember275 {
    char storage[0x80];

    __attribute__((always_inline)) ~UnkMember275() { fn_7100216934(storage); }
};

struct UnkCountedRef275 {
    char* counted;

    __attribute__((always_inline)) ~UnkCountedRef275() {
        if (counted && --*reinterpret_cast<s32*>(counted + 0x54) == 0)
            fn_710001737c(reinterpret_cast<UnkCountedResource*>(counted));
    }
};

struct UnkBase275 {
    void* vtable;
    u8 pad_0x8[0x8];
    base::UnkStrIdMember id;  // 0x10
    u8 pad_0x18[0x60];
    UnkCountedRef275 counted;  // 0x78
    UnkMember275 member;  // 0x80
    u32 field_0x100;

    __attribute__((always_inline)) ~UnkBase275() {
        vtable = &g_unk_7101a1f000[2];
        field_0x100 = 0xdeaddead;
    }
};

struct UnkHelperPtr275 {
    char* ptr;

    __attribute__((always_inline)) ~UnkHelperPtr275() {
        char* p = ptr;
        ptr = nullptr;
        if (p) {
            fn_71001be078(p);
            fn_7100083c00(p);
        }
    }
};

template <class T>
struct UnkOwnedVector275 : base::UnkVector<T> {
    __attribute__((always_inline)) ~UnkOwnedVector275() { this->Free(); }
};

struct UnkObject275 : UnkBase275 {
    UnkHelperPtr275 helper;  // 0x108
    UnkOwnedVector275<u64> items;  // 0x110

    __attribute__((always_inline)) ~UnkObject275() { vtable = &g_unk_7101a1f218[2]; }
};

void fn_71001f4efc(UnkObject275* self) { self->~UnkObject275(); }
