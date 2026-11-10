// Unit 138: a holder's teardown. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "types.h"

void fn_71001c3894(char* self, void* owner);
void fn_710020af6c(char* self);

struct UnkHolder_b0187a {
    u8 unk_0x0[4];
    u32 magic;  // 0x4
    UnkCountedResource* resource;  // 0x8
    u8 unk_0x10[8];
    u8 member[0x108];  // 0x18
    char* child;  // 0x120
};
static_assert(__builtin_offsetof(UnkHolder_b0187a, child) == 0x120, "UnkHolder_b0187a layout");

void fn_7100147df4(UnkHolder_b0187a* self) {
    char* member = reinterpret_cast<char*>(self) + 0x18;
    fn_71001c3894(self->child, nullptr);
    fn_710020af6c(member);
    UnkCountedResource* resource = self->resource;
    self->magic = 0xdeaddead;
    if (resource && --resource->ref_count == 0)
        fn_710001737c(resource);
}
