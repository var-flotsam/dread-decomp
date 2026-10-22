#pragma once

// A 0x50-byte element (a vtable, a sub-object copied by fn_71003c00b0, a flag byte and cleared
// state) of the growable array that fn_71004dccac appends to; its destructor goes through the
// vtable (slot 1), so the array's element moves call it virtually.

#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101cac938[];

// Words at 4-byte alignment inside the element.
typedef u64 u64_a4 __attribute__((aligned(4)));

struct UnkSub {
    u64 field_0x0;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 field_0x14;
    u8 pad_0x15[3];
    u32 field_0x18;
    u8 pad_0x1c[4];
    u64 field_0x20;
};

void fn_71003c00b0(UnkSub* self, const UnkSub* other);  // copy constructor

struct UnkItem {
    void** vtable;
    UnkSub field_0x8;
    u8 field_0x30;
    u8 pad_0x31[3];
    u64_a4 field_0x34;
    u64_a4 field_0x3c;
    u64_a4 field_0x44;

    UnkItem(const UnkItem& other) {
        vtable = g_unk_7101cac938 + 2;
        fn_71003c00b0(&field_0x8, &other.field_0x8);
        field_0x30 = other.field_0x30;
        field_0x34 = 0;
        field_0x3c = 0;
        field_0x44 = 0;
    }
    UnkItem(UnkItem&& other) {
        vtable = g_unk_7101cac938 + 2;
        field_0x8.field_0x0 = other.field_0x8.field_0x0;
        field_0x8.field_0x8 = other.field_0x8.field_0x8;
        field_0x8.field_0xc = other.field_0x8.field_0xc;
        field_0x8.field_0x10 = other.field_0x8.field_0x10;
        field_0x8.field_0x14 = other.field_0x8.field_0x14;
        field_0x8.field_0x18 = other.field_0x8.field_0x18;
        field_0x8.field_0x20 = other.field_0x8.field_0x20;
        u8 flag = other.field_0x30;
        __builtin_memset(&other.field_0x8, 0, 16);
        field_0x30 = flag;
        field_0x34 = 0;
        field_0x3c = 0;
        field_0x44 = 0;
    }
    ~UnkItem() { reinterpret_cast<void (*)(UnkItem*)>(vtable[1])(this); }
};

void fn_71004dccac(base::UnkVector<UnkItem>* self, const UnkItem* value);  // appends a copy
