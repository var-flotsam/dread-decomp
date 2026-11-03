// Unit 2565 (tools/tu_map.py, no name hint): the reflected setter "_aGridCellBlocks_SetterFunction" of
// the class with sScenarioId, vGridMin and vGridMax (registered by fn_7100e77a0c). Copies the u32
// elements of the argument's array into the object's array at +0x20, skipping those past the
// object's count at +0x5f5c; returns an empty value. Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

namespace {

struct UnkU32Array_b41a3 {
    u32* data;
    u32 count;
};

}  // namespace

UnkReflValue fn_7100e7c8cc(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    const UnkU32Array_b41a3* src;
    if (!a->type)
        src = nullptr;
    else if (a->field_0x14 == 1 || *reinterpret_cast<const u64*>(a->type + 0x18) > 8)
        src = static_cast<const UnkU32Array_b41a3*>(a->field_0x0);
    else
        src = reinterpret_cast<const UnkU32Array_b41a3*>(a);
    const u32* count = reinterpret_cast<const u32*>(target + 0x5f5c);
    u32* cells = reinterpret_cast<u32*>(target + 0x20);
    for (u64 i = 0; i < src->count; ++i) {
        if (i < *count)
            cells[i] = src->data[i];
    }
    return base::reflection::refl_EmptyValue();
}
