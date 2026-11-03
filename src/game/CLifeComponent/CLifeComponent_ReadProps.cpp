// CLifeComponent unit (tools/tu_map.py, the name from reflection): reflected functions that read one
// value from the stream picked from the argument's holder (none when the argument has no type; inline,
// or behind a pointer for 9 or more entries or kind 1) through fn_710007cf30 into a field of the
// component at 8, and return the int 1 as a reflected value. Names and layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkValueSet_lcr {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_lcr {
    void* holder;
    UnkValueSet_lcr* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkLifeComponent_lcr {
    u8 unk_0x0[0x64];
    bool field_0x64;
    u8 unk_0x65[3];
    f32 field_0x68;
    bool field_0x6c;
};

struct UnkLifeComponentOwner_lcr {
    u8 unk_0x0[8];
    UnkLifeComponent_lcr* target;
};

static inline __attribute__((always_inline)) void* PickArg_lcr(UnkValueSource_lcr* src) {
    if (!src->set)
        return nullptr;
    if (src->kind == 1 || src->set->count >= 9)
        return src->holder;
    return src;
}

static inline __attribute__((always_inline)) UnkReflValue ReturnOne_lcr(s32& one) {
    one = 1;
    const base::reflection::CType* type = fn_7100014768();
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    fn_71000a18d4(&r, type, &one, false);
    return r;
}

UnkReflValue fn_7101161520(void*, UnkLifeComponentOwner_lcr* owner, UnkValueSource_lcr** source) {
    UnkValueSource_lcr* src = *source;
    UnkLifeComponent_lcr* target = owner->target;
    void* stream = PickArg_lcr(src);
    bool value = false;
    s32 one;
    fn_710007cf30(stream, "b", &value);
    target->field_0x6c = value;
    return ReturnOne_lcr(one);
}

UnkReflValue fn_71011615c0(void*, UnkLifeComponentOwner_lcr* owner, UnkValueSource_lcr** source) {
    UnkValueSource_lcr* src = *source;
    UnkLifeComponent_lcr* target = owner->target;
    void* stream = PickArg_lcr(src);
    bool value = false;
    s32 one;
    fn_710007cf30(stream, "b", &value);
    target->field_0x64 = value;
    return ReturnOne_lcr(one);
}

UnkReflValue fn_7101161660(void*, UnkLifeComponentOwner_lcr* owner, UnkValueSource_lcr** source) {
    UnkValueSource_lcr* src = *source;
    UnkLifeComponent_lcr* target = owner->target;
    void* stream = PickArg_lcr(src);
    f32 value = 0.0f;
    s32 one;
    fn_710007cf30(stream, "f", &value);
    target->field_0x68 = value;
    return ReturnOne_lcr(one);
}

// Two bool arguments (the second argument follows the first, 0x18 bytes on) passed to the component's
// vf_0x180. Returns an empty result of kind 2.
struct UnkResult_0x18_lcr {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkLifeComponentV_lcr {
    void** vtable;

    using Fn = void (*)(UnkLifeComponentV_lcr*, bool, bool);
    Fn vf_0x180() { return reinterpret_cast<Fn>(vtable[0x180 / 8]); }
};

struct UnkLifeComponentVOwner_lcr {
    u8 unk_0x0[8];
    UnkLifeComponentV_lcr* target;
};

template <class T>
static inline __attribute__((always_inline)) T ArgValue_lcr(UnkValueSource_lcr* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return *static_cast<T*>(src->holder);
    return *reinterpret_cast<T*>(src);
}

UnkResult_0x18_lcr fn_7101161700(void*, UnkLifeComponentVOwner_lcr* owner, UnkValueSource_lcr** source) {
    UnkLifeComponentV_lcr* target = owner->target;
    UnkValueSource_lcr* args = *source;
    UnkLifeComponentV_lcr::Fn fn = target->vf_0x180();
    fn(target, ArgValue_lcr<u8>(&args[0]) != 0, ArgValue_lcr<bool>(&args[1]));
    UnkResult_0x18_lcr result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
