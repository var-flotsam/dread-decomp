// CCutsceneComponent's unit (tools/tu_map.py): a reflected setter that copies the argument's list of offsets (three
// words each; data at +0, count at +8) into the offset array at +0x58 (fn_71000cc374, as fn_71007a9d8c in
// CCutsceneComponent_b40.cpp). Answers the empty status {0, 0, 2, 1}. Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkVec3_cs;

struct UnkOffsetList_A5_b67 {
    const u32* data;
    u32 count;
};

static inline __attribute__((always_inline)) void* ArgPtr_A5_b67(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71007ad6c0(void*, char* obj, UnkReflValue* const* args) {
    char* self = *reinterpret_cast<char**>(obj + 8);
    const UnkOffsetList_A5_b67* list = static_cast<const UnkOffsetList_A5_b67*>(ArgPtr_A5_b67(args[0]));
    asm("" : "+r"(list));
    for (u64 i = 0; i < list->count; ++i) {
        const u32* data = list->data;
        u32* dst = reinterpret_cast<u32*>(fn_71000cc374(self + 0x58, i));
        const u32* src = data + i * 3;
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
    }
    return base::reflection::refl_EmptyValue();
}
