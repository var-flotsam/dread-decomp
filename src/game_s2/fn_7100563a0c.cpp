// Constructors (fn_7100563a0c, fn_710056264c) of an object made of two sub-objects, each with
// its own vtable (rewritten by the second constructor step) and arrays created with allocator
// flags 0xf. The two classes differ in their vtables and in how many arrays each sub-object has.
#include <new>
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a48298[];
extern void* g_unk_7101a48270[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a20950[];

struct UnkLayerA {
    void** vtable;
    base::UnkVector<u64> vec1;
    u32 field_0x38;
    u64 field_0x40;
    base::UnkVector<u64> vec2;
};

struct UnkLayerA2 {
    void** vtable;
    base::UnkVector<u64> vec1;
    u32 field_0x38;
    u64 field_0x40;
};

struct UnkLayerB {
    void** vtable;
    base::UnkVector<u64> vec1;
    u32 field_0xa0;
    u64 field_0xa8;
    u8 field_0xb0;
    base::UnkVector<u64> vec2;
};

struct UnkLayerB2 {
    void** vtable;
    base::UnkVector<u64> vec1;
    u32 field_0x78;
    u64 field_0x80;
    base::UnkVector<u64> vec2;
    base::UnkVector<u64> vec3;
    u8 field_0xd8;
};

struct UnkTwoPart {
    void** vtable;
    UnkLayerA a;
    UnkLayerB b;
};

void fn_7100563a0c(UnkTwoPart* self) {
    self->vtable = &g_unk_7101a48298[2];
    self->a.vtable = &g_unk_7101a20998[2];
    new (&self->a.vec1) base::UnkVector<u64>(0xf);
    self->a.vtable = &g_unk_7101a20950[2];
    self->a.field_0x38 = 0;
    self->a.field_0x40 = 0;
    new (&self->a.vec2) base::UnkVector<u64>(0xf);
    self->b.vtable = &g_unk_7101a20998[2];
    new (&self->b.vec1) base::UnkVector<u64>(0xf);
    self->b.field_0xa0 = 0;
    self->b.vtable = &g_unk_7101a20950[2];
    self->b.field_0xa8 = 0;
    self->b.field_0xb0 = 0;
    new (&self->b.vec2) base::UnkVector<u64>(0xf);
}

struct UnkTwoPart2 {
    void** vtable;
    UnkLayerA2 a;
    UnkLayerB2 b;
};

void fn_710056264c(UnkTwoPart2* self) {
    self->vtable = &g_unk_7101a48270[2];
    self->a.vtable = &g_unk_7101a20998[2];
    new (&self->a.vec1) base::UnkVector<u64>(0xf);
    self->a.vtable = &g_unk_7101a20950[2];
    self->a.field_0x38 = 0;
    self->a.field_0x40 = 0;
    self->b.vtable = &g_unk_7101a20998[2];
    new (&self->b.vec1) base::UnkVector<u64>(0xf);
    self->b.field_0x78 = 0;
    self->b.vtable = &g_unk_7101a20950[2];
    self->b.field_0x80 = 0;
    new (&self->b.vec2) base::UnkVector<u64>(0xf);
    new (&self->b.vec3) base::UnkVector<u64>(0xf);
    self->b.field_0xd8 = 0;
}
