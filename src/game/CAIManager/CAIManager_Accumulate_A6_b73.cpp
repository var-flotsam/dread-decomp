// CAIManager's unit (tools/tu_map.py; the name from reflection): after fn_7100dada0c, when the
// object at +0x118 has something at +0x10, walk the packed list at +8 (u32 count, then the
// pointers from +0xc). Each non-null entry is held (count at +8) while its object at +0x10 is
// tested with fn_710110c83c; a hit adds `delta` to the float at +0x120. Dropping the last hold
// calls the entry's slot 0x10. Names and layouts are placeholders.
#include "types.h"

namespace {

struct UnkHeldEntry_A6b73 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 refs;     // 0x8
    void* object;  // 0x10
};

}  // namespace

void fn_7100dada0c(char* self);
bool fn_710110c83c(void* object);

void fn_7100dad940(char* self, float delta) {
    fn_7100dada0c(self);
    char* owner = *reinterpret_cast<char**>(self + 0x118);
    if (!owner || !*reinterpret_cast<void**>(owner + 0x10))
        return;
    u64 bytes = (u64)*reinterpret_cast<u32*>(self + 8) * 8;
    asm("" : "+r"(bytes));  // the original counts the byte size down
    if (!bytes)
        return;
    UnkHeldEntry_A6b73** it = reinterpret_cast<UnkHeldEntry_A6b73**>(self + 0xc);
    for (; bytes; ++it, bytes -= 8) {
        UnkHeldEntry_A6b73* e = *it;
        if (!e)
            continue;
        ++e->refs;
        if (e->object && fn_710110c83c(e->object))
            *reinterpret_cast<float*>(self + 0x120) += delta;
        if (--*reinterpret_cast<s32*>(&e->refs) == 0)
            e->vf_0x10();
    }
}
