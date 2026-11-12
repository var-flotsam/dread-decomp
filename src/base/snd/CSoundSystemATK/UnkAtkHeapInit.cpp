// From unit 99 (base::snd::CSoundSystemATK): the set-up of an ATK heap's sub-allocator, the shape
// fn_71000f7800 inlines for its two "ATKHeap" heaps (see UnkAtkHeap in CSoundSystemATK.cpp): the
// sub-allocator over [begin, end), nn::atk's heap over the same range, then the bookkeeping words
// cleared. `self` is the sub-allocator at +0x8 of that heap. Names are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "game/UnkTU2772.h"
#include "types.h"

struct UnkAtkSubHeap {
    base::UnkSubAllocator allocator;
    u8 heap[8];      // 0x38, nn::atk's
    void* last;      // 0x40
    void* hook;      // 0x48
    void* hook_arg;  // 0x50
    u64 field_0x58;
    u64 field_0x60;
};
static_assert(__builtin_offsetof(UnkAtkSubHeap, field_0x60) == 0x60, "UnkAtkSubHeap placeholder layout");

void fn_7100103900(UnkAtkSubHeap* self, char* begin, char* end, const char* name, u64 arg) {
    fn_7100081330(&self->allocator, begin, end, name, arg);
    fn_710120b198(self->heap, begin, end - begin);
    self->field_0x58 = 0;
    self->field_0x60 = 0;
    self->last = nullptr;
    self->hook = nullptr;
    self->hook_arg = nullptr;
}
