#pragma once

// Records of properties whose copy constructor and copy-assignment the game has out of line
// (fn_7100442780, fn_710043439c, fn_710048b740, fn_7100510548, fn_71004c8f58 and their copy
// constructors): a few header properties, then 0x70-byte entries, each a property holding an
// any-like value (copied by fn_71000a18d4, assigned by fn_710009efb8) and a 0x40-byte block. Each
// record's special members are defaulted; the layout alone decides how Clang copies it:
// - runs of trivially copyable fields become one memcpy, lowered to loads and stores;
// - fields of a type with a user operator= (UnkVal, UnkTail) are assigned one by one, in order;
//   their user copy constructors keep them out of the constructor's memcpy runs too;
// - an 8-byte gap that is never copied is the vptr of a polymorphic member;
// - the entries are separate members: an array of them would be copied by a loop.
// Names and layouts are placeholders.

#include "game_s2/UnkWeakHandle.h"
#include "types.h"

namespace unk_any_props {

struct UnkAnyType {
    u8 pad_0x0[0x18];
    u64 size;
};

struct UnkAny;
void fn_71000a18d4(UnkAny* any, UnkAnyType* type, const void* data, u32 flag);

struct UnkAny {
    u64 storage;
    UnkAnyType* type;
    u32 field_0x10;
    u32 field_0x14;

    // Values of up to 8 bytes are stored inline unless field_0x14 is 1.
    const void* Data() const {
        if (field_0x14 == 1 || type->size > 8)
            return *reinterpret_cast<void* const*>(&storage);
        return &storage;
    }
    UnkAny(const UnkAny& o) : storage(0), type(nullptr) {
        field_0x10 = o.field_0x10;
        fn_71000a18d4(this, o.type, o.type ? o.Data() : nullptr, 0);
    }
    UnkAny& operator=(const UnkAny& o);  // fn_710009efb8
};

struct UnkWeakRef {
    UnkWeakHandle* h;
    UnkWeakRef(const UnkWeakRef& o) : h(nullptr) { AssignRef(&h, o.h); }
    // Out of line without the attribute.
    __attribute__((always_inline)) UnkWeakRef& operator=(const UnkWeakRef& o) {
        if (this != &o)
            CopyWeakRef(&h, o.h);
        return *this;
    }
};

template <class T, int N>
struct UnkFixedVec {
    u32 count;
    T data[N];

    UnkFixedVec(const UnkFixedVec& o) {
        __builtin_memcpy(data, o.data, o.count * sizeof(T));
        count = o.count;
    }
    UnkFixedVec& operator=(const UnkFixedVec& o) {
        if (this != &o) {
            __builtin_memcpy(data, o.data, o.count * sizeof(T));
            count = o.count;
        }
        return *this;
    }
};

struct UnkPair32 {
    u32 a;
    u32 b;
};

// A value copied on its own rather than as part of a memcpy run.
template <class T>
struct UnkVal {
    T v;

    UnkVal(const UnkVal& o) : v(o.v) {}
    UnkVal& operator=(const UnkVal& o) {
        v = o.v;
        return *this;
    }
};

struct UnkBlock10 {
    u64 v[2];
};

struct UnkBlock20 {
    u64 v[4];
};

struct UnkBlock40 {
    u64 v[8];

    // Copied element by element in the copy constructors, as one run in the assignments.
    UnkBlock40(const UnkBlock40& o) {
        for (int i = 0; i < 8; i++)
            v[i] = o.v[i];
    }
};

struct UnkPropRef {
    virtual void vf_0x0();
    u64 field_0x8;
    u64 field_0x10;
    UnkWeakRef ref;

    // The constructors copy the two values one by one.
    UnkPropRef(const UnkPropRef& o) : field_0x8(o.field_0x8), field_0x10(o.field_0x10), ref(o.ref) {}
};
static_assert(sizeof(UnkPropRef) == 0x20, "");

// Ends after the u16: a record's next fields are copied as a separate run.
struct UnkPropList {
    virtual void vf_0x0();
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    UnkFixedVec<UnkPair32, 16> list;
    u32 field_0xa4;
    u16 field_0xa8;
};
static_assert(sizeof(UnkPropList) == 0xb0, "");

struct UnkPropAny {
    virtual void vf_0x0();
    u64 field_0x8;
    u32 field_0x10;
    UnkAny value;
};
static_assert(sizeof(UnkPropAny) == 0x30, "");

struct UnkEntry : UnkPropAny {
    void vf_0x0() override;
    UnkBlock40 block;
};
static_assert(sizeof(UnkEntry) == 0x70, "");

// The base of every record (vtable 0x7101cd1b38's).
struct UnkRecordBase {
    virtual void vf_0x0();
    u64 field_0x8[4];
};

// The tail of fn_7100442780's, fn_710043439c's and fn_710048b740's records: each field copied
// on its own.
struct UnkTail {
    u16 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
    u32 field_0x7c;
    u32 field_0x80;
    u32 field_0x84;
    u64 field_0x88;

    UnkTail(const UnkTail& o)
        : field_0x0(o.field_0x0),
          field_0x4(o.field_0x4),
          field_0x8(o.field_0x8),
          field_0xc(o.field_0xc),
          field_0x10(o.field_0x10),
          field_0x14(o.field_0x14),
          field_0x18(o.field_0x18),
          field_0x1c(o.field_0x1c),
          field_0x20(o.field_0x20),
          field_0x24(o.field_0x24),
          field_0x28(o.field_0x28),
          field_0x2c(o.field_0x2c),
          field_0x30(o.field_0x30),
          field_0x34(o.field_0x34),
          field_0x38(o.field_0x38),
          field_0x3c(o.field_0x3c),
          field_0x40(o.field_0x40),
          field_0x44(o.field_0x44),
          field_0x48(o.field_0x48),
          field_0x4c(o.field_0x4c),
          field_0x50(o.field_0x50),
          field_0x54(o.field_0x54),
          field_0x58(o.field_0x58),
          field_0x5c(o.field_0x5c),
          field_0x60(o.field_0x60),
          field_0x64(o.field_0x64),
          field_0x68(o.field_0x68),
          field_0x6c(o.field_0x6c),
          field_0x70(o.field_0x70),
          field_0x74(o.field_0x74),
          field_0x78(o.field_0x78),
          field_0x7c(o.field_0x7c),
          field_0x80(o.field_0x80),
          field_0x84(o.field_0x84),
          field_0x88(o.field_0x88) {}
    UnkTail& operator=(const UnkTail& o) {
        field_0x0 = o.field_0x0;
        field_0x4 = o.field_0x4;
        field_0x8 = o.field_0x8;
        field_0xc = o.field_0xc;
        field_0x10 = o.field_0x10;
        field_0x14 = o.field_0x14;
        field_0x18 = o.field_0x18;
        field_0x1c = o.field_0x1c;
        field_0x20 = o.field_0x20;
        field_0x24 = o.field_0x24;
        field_0x28 = o.field_0x28;
        field_0x2c = o.field_0x2c;
        field_0x30 = o.field_0x30;
        field_0x34 = o.field_0x34;
        field_0x38 = o.field_0x38;
        field_0x3c = o.field_0x3c;
        field_0x40 = o.field_0x40;
        field_0x44 = o.field_0x44;
        field_0x48 = o.field_0x48;
        field_0x4c = o.field_0x4c;
        field_0x50 = o.field_0x50;
        field_0x54 = o.field_0x54;
        field_0x58 = o.field_0x58;
        field_0x5c = o.field_0x5c;
        field_0x60 = o.field_0x60;
        field_0x64 = o.field_0x64;
        field_0x68 = o.field_0x68;
        field_0x6c = o.field_0x6c;
        field_0x70 = o.field_0x70;
        field_0x74 = o.field_0x74;
        field_0x78 = o.field_0x78;
        field_0x7c = o.field_0x7c;
        field_0x80 = o.field_0x80;
        field_0x84 = o.field_0x84;
        field_0x88 = o.field_0x88;
        return *this;
    }
};
static_assert(sizeof(UnkTail) == 0x90, "");

}  // namespace unk_any_props
