// Setup of an object holding a sub-allocator at 8 (base::UnkSubAllocator) and a second heap at
// 0x40 over the same range (fn_710120b198, middleware), with the words after it cleared
// (fn_71000f8630). It has no direct callers.
#include "base/UnkDefaultAllocator.h"
#include "types.h"

void fn_710120b198(void* heap, char* begin, u64 size);

struct UnkTwoHeaps_71000f8630 {
    u64 field_0x0;
    base::UnkSubAllocator sub;
    u8 heap_0x40[8];
    u64 field_0x48;
    u64 field_0x50;
    u64 field_0x58;
    u64 field_0x60;
    u64 field_0x68;
};
static_assert(offsetof(UnkTwoHeaps_71000f8630, heap_0x40) == 0x40, "layout");

void fn_71000f8630(UnkTwoHeaps_71000f8630* self, char* begin, char* end, const char* name, u64 arg) {
    fn_7100081330(&self->sub, begin, end, name, arg);
    fn_710120b198(self->heap_0x40, begin, end - begin);
    self->field_0x48 = 0;
    self->field_0x50 = 0;
    self->field_0x58 = 0;
    self->field_0x60 = 0;
    self->field_0x68 = 0;
}
