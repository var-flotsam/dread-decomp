// Unit 426 of tools/tu_map.py: the destructor of an object with a second base at +0x8. It drops
// its counted resource (+0x90), clears the member at +0x68 (fn_71002f7930) and frees its two
// buffers through the allocator at +0x88, then does the same for the second base's member at
// +0x10 (fn_71002f6ac0, the buffer at +0x10, the allocator at +0x30). Names and layouts are
// placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "game/UnkTU426.h"
#include "types.h"

extern char g_unk_7101a222e8[];
extern void* g_unk_7101a22398[];

struct UnkUnit426Member68 {
    void* buffer_b;  // 0x68
    void* buffer_a;  // 0x70
};

struct UnkUnit426Object {
    void* vtable;
    void* vtable2;               // 0x8, the second base
    void* buffer;                // 0x10
    u32 field_0x18;
    u32 field_0x1c;
    u8 pad_0x20[0x10];
    base::UnkAllocator* allocator2;  // 0x30
    u8 pad_0x38[0x30];
    UnkUnit426Member68 member;   // 0x68
    u8 pad_0x78[0x10];
    base::UnkAllocator* allocator;  // 0x88
    UnkCountedResource* resource;   // 0x90
};

void fn_71002f7930(UnkUnit426Member68* member);

void fn_71002f650c(UnkUnit426Object* self) {
    self->vtable = g_unk_7101a222e8 + 0x10;
    self->vtable2 = g_unk_7101a222e8 + 0x40;
    if (UnkCountedResource* r = self->resource) {
        if (--r->ref_count == 0)
            fn_710001737c(r);
    }
    fn_71002f7930(&self->member);
    if (self->member.buffer_a)
        self->allocator->vf_0x48(self->member.buffer_a);
    if (self->member.buffer_b)
        self->allocator->vf_0x48(self->member.buffer_b);
    self->vtable2 = reinterpret_cast<char*>(g_unk_7101a22398) + 0x10;
    fn_71002f6ac0(&self->buffer);
    if (self->buffer) {
        self->field_0x18 = 0;
        self->allocator2->vf_0x48(self->buffer);
        self->field_0x1c = 0;
    }
}
