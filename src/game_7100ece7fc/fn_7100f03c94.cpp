// Destructors (complete and deleting) of an object holding an array of polymorphic 0x18-byte
// elements: restore the vtable, destroy the elements through their vtables and free the array
// (the deleting one then frees the object).
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkPolyElem_0x18 {
    virtual ~UnkPolyElem_0x18();
    u64 field_0x8;
    u64 field_0x10;
};

extern void* g_unk_7101caa380[];
extern void* g_unk_7101a3e090[];
extern void* g_unk_7101a3e2d8[];

// The two copies' elements have their destructor in the second vtable slot.
struct UnkPolyElem_0x18B {
    virtual void vf_0x0();
    virtual ~UnkPolyElem_0x18B();
    u64 field_0x8;
    u64 field_0x10;
};

// And a third copy's 0x40-byte elements.
struct UnkPolyElem_0x40 {
    virtual ~UnkPolyElem_0x40();
    u8 unk_0x8[0x38];
};

extern void* g_unk_7101a717e8[];

template <void** Vtable, u32 At = 0x20, class Elem = UnkPolyElem_0x18>
struct UnkPolyArrayOwner {
    void** vtable;
    u8 unk_0x8[At - 0x8];
    base::UnkVector<Elem> elems;

    ~UnkPolyArrayOwner() {
        vtable = &Vtable[2];
        elems.Free();
    }
};

using UnkPolyArrayOwner_7100f03c94 = UnkPolyArrayOwner<g_unk_7101caa380>;

#define UNK_POLY_ARRAY_DTORS(complete, deleting, Owner)                \
    void fn_##complete(Owner& self) { self.~UnkPolyArrayOwner(); } \
    void fn_##deleting(Owner& self) { delete &self; }

using UnkPolyArrayOwner_71004abbc8 = UnkPolyArrayOwner<g_unk_7101a3e090, 0x18, UnkPolyElem_0x18B>;
using UnkPolyArrayOwner_71004b5d14 = UnkPolyArrayOwner<g_unk_7101a3e2d8, 0x8, UnkPolyElem_0x18B>;
using UnkPolyArrayOwner_71007cce28 = UnkPolyArrayOwner<g_unk_7101a717e8, 0x20, UnkPolyElem_0x40>;

UNK_POLY_ARRAY_DTORS(7100f03be8, 7100f03c94, UnkPolyArrayOwner_7100f03c94)
// The same pair for two classes in other units, with their own vtables and the array at 0x18 and 0x8.
UNK_POLY_ARRAY_DTORS(71004abb1c, 71004abbc8, UnkPolyArrayOwner_71004abbc8)
UNK_POLY_ARRAY_DTORS(71004b5c68, 71004b5d14, UnkPolyArrayOwner_71004b5d14)
// And for a class with 0x40-byte elements.
UNK_POLY_ARRAY_DTORS(71007ccd7c, 71007cce28, UnkPolyArrayOwner_71007cce28)
