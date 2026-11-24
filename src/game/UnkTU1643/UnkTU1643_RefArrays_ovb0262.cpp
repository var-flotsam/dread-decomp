// Unit 1643: a record holding three reference-counted pointer arrays and its owner, which clears an
// inline array of such records plus three more arrays of its own. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CRntVectorReloading.h"
#include "base/global/CStrId.h"
#include "game/UnkTU107.h"
#include "types.h"

struct UnkRefObj_ovb0262a {
    s32 ref_count;
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

// The original reloads the count after each release (base/global/CRntVectorReloading.h).
struct UnkRefArray_ovb0262a : base::global::CRntVectorReloading<UnkRefObj_ovb0262a*> {
    // Releases the references and frees the array.
    inline void Free() {
        if (data) {
            for (u64 i = 0; i < count; ++i) {
                UnkRefObj_ovb0262a* o = data[i];
                if (o && --o->ref_count == 0)
                    o->vf_0x10();
            }
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkHolder_ovb0262a {
    base::global::CStrId id;  // 0x0
    u8 pad_0x8[0x8];
    UnkRefArray_ovb0262a a;  // 0x10
    u8 pad_0x38[0x8];
    UnkRefArray_ovb0262a b;  // 0x40
    UnkRefArray_ovb0262a c;  // 0x68
};

__attribute__((noinline)) void fn_7100ae01d0(UnkHolder_ovb0262a* self) {
    self->c.Free();
    self->b.Free();
    self->a.Free();
    self->id.Reset();
}

struct UnkPlainArray_ovb0262a {
    void* data;
    union {
        u32 count;
    };
    u32 capacity;
};

struct UnkOwner_ovb0262a {
    base::global::CStrId id;  // 0x0
    UnkRefArray_ovb0262a a;  // 0x8
    UnkRefArray_ovb0262a b;  // 0x30
    u8 pad_0x58[0x8];
    UnkPlainArray_ovb0262a plain;  // 0x60
    u8 pad_0x70[0x10];
    base::IAllocator* alloc_plain;  // 0x80
    u8 pad_0x88[0x8];
    UnkRefArray_ovb0262a d;  // 0x90
    u32 element_count;  // 0xb8 (placed below)
    u8 elements[0x90];  // 0xbc
};

void fn_7100adffac(UnkOwner_ovb0262a* self) {
    u8* element = self->elements;
    for (u64 i = 0; i < self->element_count; ++i, element += 0x90)
        fn_7100ae01d0(reinterpret_cast<UnkHolder_ovb0262a*>(element));
    self->element_count = 0;
    self->d.Free();
    fn_710010d9f4(&self->plain);
    if (self->plain.data) {
        self->plain.count = 0;
        self->alloc_plain->vf_0x48(self->plain.data);
        self->plain.capacity = 0;
    }
    self->b.Free();
    self->a.Free();
    self->id.Reset();
}
