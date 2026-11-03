// GUI::CDisplayObject unit (tools/tu_map.py, the name from reflection): reflected setters for the four
// floats at +0xa0..+0xac. Each takes the float picked from the argument's holder (inline, or behind a
// pointer for 9 or more entries or kind 1), copies it into two slots of the block fn_71011ce37c hands out
// (when there is one) and, when the object's own value changes, stores it and tells the object (vf_0xa0,
// the first time), marks the derived flags stale and sets change bit 0x20. Each returns an empty result
// of kind 2. Names and layouts are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkValueSet_gdc {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_gdc {
    void* holder;
    UnkValueSet_gdc* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_gdc {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkGuiOwner_gdc {
    u8 unk_0x0[8];
    UnkGuiDisplayObject* target;
};

// The block of the four values (two copies of each, at +0xc and +0x1c), or null.
char* fn_71011ce37c(char*);

static inline __attribute__((always_inline)) f32 PickArg_gdc(UnkValueSource_gdc* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return *static_cast<f32*>(src->holder);
    return *reinterpret_cast<f32*>(src);
}

template <u32 Index>
static inline __attribute__((always_inline)) UnkResult_0x18_gdc SetValue_gdc(UnkGuiOwner_gdc* owner,
                                                                             UnkValueSource_gdc** source) {
    UnkValueSource_gdc* src = *source;
    UnkGuiDisplayObject* target = owner->target;
    f32 value = PickArg_gdc(src);
    if (char* block = fn_71011ce37c(reinterpret_cast<char*>(target))) {
        reinterpret_cast<f32*>(block + 0x1c)[Index] = value;
        reinterpret_cast<f32*>(block + 0xc)[Index] = value;
    }
    f32* field = reinterpret_cast<f32*>(reinterpret_cast<char*>(target) + 0xa0) + Index;
    if (*field != value) {
        *field = value;
        if (!target->field_0x184) {
            target->field_0x184 = true;
            target->vf_0xa0();
        }
        target->field_0x183 = true;
        target->field_0x144 |= 0x20;
    }
    UnkResult_0x18_gdc result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

UnkResult_0x18_gdc fn_71011d52ac(void*, UnkGuiOwner_gdc* owner, UnkValueSource_gdc** source) {
    return SetValue_gdc<0>(owner, source);
}
UnkResult_0x18_gdc fn_71011d53b0(void*, UnkGuiOwner_gdc* owner, UnkValueSource_gdc** source) {
    return SetValue_gdc<1>(owner, source);
}
UnkResult_0x18_gdc fn_71011d54b4(void*, UnkGuiOwner_gdc* owner, UnkValueSource_gdc** source) {
    return SetValue_gdc<2>(owner, source);
}
UnkResult_0x18_gdc fn_71011d55b8(void*, UnkGuiOwner_gdc* owner, UnkValueSource_gdc** source) {
    return SetValue_gdc<3>(owner, source);
}
