// CActor unit (tools/tu_map.py, hint from reflection): reflected bool setters called through
// function invokers (src/reflection/function_inits_00.cpp). Each reads the bool argument from its
// value holder and stores it in the object at +8 when it changes: fn_710023e8f0 then tells the
// object through vf_0x138 (set) or vf_0x140 (cleared) unless a flag at +0x1b0 is set with nothing
// at +0x1a8, and always through vf_0x110; fn_710023e9e4 passes it to vf_0xe8 of every object in
// the array at +0xa0. Both answer the empty status {0, 0, 2, 1}. Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkVirtual_b60 {
    void** vtable;

    template <u32 Offset, class... Args>
    void Call(Args... args) {
        reinterpret_cast<void (*)(UnkVirtual_b60*, Args...)>(vtable[Offset / 8])(this, args...);
    }
};

struct UnkActorFlags_b60 : UnkVirtual_b60 {
    u8 pad_0x8[0x98];
    UnkVirtual_b60** children;  // 0xa0
    u8 pad_0xa8[0x8];
    u32 child_count;  // 0xb0
    u8 pad_0xb4[0xf4];
    void* field_0x1a8;
    bool field_0x1b0;
    u8 pad_0x1b1[0x2];
    bool field_0x1b3;
    bool field_0x1b4;
};

static inline __attribute__((always_inline)) const bool* BoolArg_b60(const UnkReflValue* v) {
    if (v->field_0x14 == 1 || *reinterpret_cast<const u64*>(v->type + 0x18) >= 9)
        return static_cast<const bool*>(v->field_0x0);
    return reinterpret_cast<const bool*>(v);
}

static inline __attribute__((always_inline)) UnkReflValue EmptyStatus_b60() {
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}

UnkReflValue fn_710023e8f0(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* self = *reinterpret_cast<UnkActorFlags_b60**>(obj + 8);
    bool value = *BoolArg_b60(a);
    if (self->field_0x1b4 != value) {
        self->field_0x1b4 = value;
        if (!self->field_0x1b0 || self->field_0x1a8) {
            if (value)
                self->Call<0x138>();
            else
                self->Call<0x140>();
        }
        self->Call<0x110>(self->field_0x1b4);
    }
    return EmptyStatus_b60();
}

UnkReflValue fn_710023e9e4(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* self = *reinterpret_cast<UnkActorFlags_b60**>(obj + 8);
    bool value = *BoolArg_b60(a);
    if (self->field_0x1b3 != value) {
        self->field_0x1b3 = value;
        for (UnkVirtual_b60 **it = self->children, **end = it + self->child_count; it != end; ++it)
            (*it)->Call<0xe8>(self->field_0x1b3);
    }
    return EmptyStatus_b60();
}
