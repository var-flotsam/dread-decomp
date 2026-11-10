// Unit 1643: a record holding three reference-counted pointer arrays and its owner, which clears an
// inline array of such records plus three more arrays of its own. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkRefObj_ovb0262a {
    s32 ref_count;
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkRefArray_ovb0262a {
    UnkRefObj_ovb0262a** data;
    union {
        u32 count;
    };
    u32 capacity;
    inline void Clear(base::UnkAllocator* const* allocator) {
        if (data) {
            for (u64 i = 0; i < count; ++i) {
                UnkRefObj_ovb0262a* o = data[i];
                if (o && --o->ref_count == 0)
                    o->vf_0x10();
            }
            count = 0;
            (*allocator)->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkHolder_ovb0262a {
    base::global::CStrId id;  // 0x0
    u8 pad_0x8[0x8];
    UnkRefArray_ovb0262a a;  // 0x10
    u8 pad_0x20[0x10];
    base::UnkAllocator* alloc_a;  // 0x30
    u8 pad_0x38[0x8];
    UnkRefArray_ovb0262a b;  // 0x40
    u8 pad_0x50[0x10];
    base::UnkAllocator* alloc_b;  // 0x60
    UnkRefArray_ovb0262a c;  // 0x68
    u8 pad_0x78[0x10];
    base::UnkAllocator* alloc_c;  // 0x88
};

__attribute__((noinline)) void fn_7100ae01d0(UnkHolder_ovb0262a* self) {
    self->c.Clear(&self->alloc_c);
    self->b.Clear(&self->alloc_b);
    self->a.Clear(&self->alloc_a);
    self->id.Reset();
}

void fn_710010d9f4(void* p);

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
    u8 pad_0x18[0x10];
    base::UnkAllocator* alloc_a;  // 0x28
    UnkRefArray_ovb0262a b;  // 0x30
    u8 pad_0x40[0x10];
    base::UnkAllocator* alloc_b;  // 0x50
    u8 pad_0x58[0x8];
    UnkPlainArray_ovb0262a plain;  // 0x60
    u8 pad_0x70[0x10];
    base::UnkAllocator* alloc_plain;  // 0x80
    u8 pad_0x88[0x8];
    UnkRefArray_ovb0262a d;  // 0x90
    u8 pad_0xa0[0x10];
    base::UnkAllocator* alloc_d;  // 0xb0
    u32 element_count;  // 0xb8 (placed below)
    u8 elements[0x90];  // 0xbc
};

void fn_7100adffac(UnkOwner_ovb0262a* self) {
    u8* element = self->elements;
    for (u64 i = 0; i < self->element_count; ++i, element += 0x90)
        fn_7100ae01d0(reinterpret_cast<UnkHolder_ovb0262a*>(element));
    self->element_count = 0;
    self->d.Clear(&self->alloc_d);
    fn_710010d9f4(&self->plain);
    if (self->plain.data) {
        self->plain.count = 0;
        self->alloc_plain->vf_0x48(self->plain.data);
        self->plain.capacity = 0;
    }
    self->b.Clear(&self->alloc_b);
    self->a.Clear(&self->alloc_a);
    self->id.Reset();
}
