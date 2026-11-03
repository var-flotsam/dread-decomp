// GUI::CDisplayObject unit (unit 2747 of tools/tu_map.py, the name from reflection): reflected property
// setters for two parameter blocks the object hands out (fn_71011ce0d8 and fn_71011ce24c). Each takes the
// value picked from the argument's holder (inline, or behind a pointer for 9 or more entries or kind 1),
// stores it in the block and tells the object of the change (vf_0xa0, the first time, or with the block's
// own dirty flag at +8). Each returns an empty result of kind 2. Names and layouts are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkValueSet_gdo {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_gdo {
    void* holder;
    UnkValueSet_gdo* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_gdo {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkGuiOwner_gdo {
    u8 unk_0x0[8];
    UnkGuiDisplayObject* target;
};

// A parameter block: a dirty flag at +8, then the values.
struct UnkGuiParams_gdo {
    u8 unk_0x0[8];
    bool dirty;
};

UnkGuiParams_gdo* fn_71011ce0d8(UnkGuiDisplayObject* self);
UnkGuiParams_gdo* fn_71011ce24c(UnkGuiDisplayObject* self);

template <class T>
static inline __attribute__((always_inline)) T PickArg_gdo(UnkValueSource_gdo* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return *static_cast<T*>(src->holder);
    return *reinterpret_cast<T*>(src);
}

static inline __attribute__((always_inline)) UnkResult_0x18_gdo Done_gdo() {
    UnkResult_0x18_gdo result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

template <UnkGuiParams_gdo* (*Get)(UnkGuiDisplayObject*), u32 Offset>
static inline __attribute__((always_inline)) UnkResult_0x18_gdo SetWord_gdo(UnkGuiOwner_gdo* owner,
                                                                             UnkValueSource_gdo** source) {
    UnkValueSource_gdo* src = *source;
    UnkGuiDisplayObject* target = owner->target;
    u32 value = PickArg_gdo<u32>(src);
    *reinterpret_cast<u32*>(reinterpret_cast<char*>(Get(target)) + Offset) = value;
    if (!target->field_0x184) {
        target->field_0x184 = true;
        target->vf_0xa0();
    }
    return Done_gdo();
}

UnkResult_0x18_gdo fn_71011d58c4(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    return SetWord_gdo<fn_71011ce0d8, 0xc>(owner, source);
}
UnkResult_0x18_gdo fn_71011d59ac(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    return SetWord_gdo<fn_71011ce0d8, 0x10>(owner, source);
}
UnkResult_0x18_gdo fn_71011d5a94(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    return SetWord_gdo<fn_71011ce0d8, 0x14>(owner, source);
}
UnkResult_0x18_gdo fn_71011d5b7c(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    return SetWord_gdo<fn_71011ce0d8, 0x18>(owner, source);
}
UnkResult_0x18_gdo fn_71011d5e14(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    return SetWord_gdo<fn_71011ce24c, 0x1c>(owner, source);
}
UnkResult_0x18_gdo fn_71011d5efc(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    return SetWord_gdo<fn_71011ce24c, 0x20>(owner, source);
}
UnkResult_0x18_gdo fn_71011d5fe4(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    return SetWord_gdo<fn_71011ce24c, 0x24>(owner, source);
}
UnkResult_0x18_gdo fn_71011d60cc(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    return SetWord_gdo<fn_71011ce24c, 0x28>(owner, source);
}

// Stores the word at +0x20 of the first block without telling the object.
UnkResult_0x18_gdo fn_71011d5d58(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    UnkValueSource_gdo* src = *source;
    UnkGuiDisplayObject* target = owner->target;
    u32 value = PickArg_gdo<u32>(src);
    *reinterpret_cast<u32*>(reinterpret_cast<char*>(fn_71011ce0d8(target)) + 0x20) = value;
    return Done_gdo();
}

// Stores the float at +0x1c of the first block; turning it positive marks the block dirty, and the
// object is told in any case.
UnkResult_0x18_gdo fn_71011d5c64(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    UnkValueSource_gdo* src = *source;
    UnkGuiDisplayObject* target = owner->target;
    f32 value = PickArg_gdo<f32>(src);
    UnkGuiParams_gdo* params = fn_71011ce0d8(target);
    f32* field = reinterpret_cast<f32*>(reinterpret_cast<char*>(params) + 0x1c);
    f32 old = *field;
    *field = value;
    if (value > 0.0f && old <= 0.0f)
        params->dirty = true;
    target->vf_0xa0();
    return Done_gdo();
}

// Stores the word at +0x30 of the second block and marks it dirty.
UnkResult_0x18_gdo fn_71011d61b4(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    UnkValueSource_gdo* src = *source;
    UnkGuiDisplayObject* target = owner->target;
    u32 value = PickArg_gdo<u32>(src);
    UnkGuiParams_gdo* params = fn_71011ce24c(target);
    *reinterpret_cast<u32*>(reinterpret_cast<char*>(params) + 0x30) = value;
    params->dirty = true;
    target->vf_0xa0();
    return Done_gdo();
}

// The object's two 2D vectors at +0x8c and +0x94, each set one component at a time.
struct UnkGuiVec2_gdo {
    f32 x;
    f32 y;
};

// The comparison tests the component that was not set first.
template <int Component>
static inline __attribute__((always_inline)) bool Differs_gdo(const UnkGuiVec2_gdo& a, const UnkGuiVec2_gdo& b) {
    if (Component == 0)
        return a.y - b.y != 0.0f || a.x - b.x != 0.0f;
    return a.x - b.x != 0.0f || a.y - b.y != 0.0f;
}

template <u32 Offset, int Component, u32 Bits>
static inline __attribute__((always_inline)) UnkResult_0x18_gdo SetComponent_gdo(UnkGuiOwner_gdo* owner,
                                                                                  UnkValueSource_gdo** source) {
    UnkValueSource_gdo* src = *source;
    UnkGuiDisplayObject* target = owner->target;
    f32 value = PickArg_gdo<f32>(src);
    UnkGuiVec2_gdo* vec = reinterpret_cast<UnkGuiVec2_gdo*>(reinterpret_cast<char*>(target) + Offset);
    UnkGuiVec2_gdo next = *vec;
    if (Component == 0)
        next.x = value;
    else
        next.y = value;
    if (Differs_gdo<Component>(next, *vec)) {
        *vec = next;
        if (!target->field_0x184) {
            target->field_0x184 = true;
            target->vf_0xa0();
        }
        target->field_0x183 = true;
        target->field_0x144 |= Bits;
    }
    return Done_gdo();
}

UnkResult_0x18_gdo fn_71011d56bc(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    return SetComponent_gdo<0x8c, 0, 4>(owner, source);
}
UnkResult_0x18_gdo fn_71011d57b8(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    return SetComponent_gdo<0x8c, 1, 4>(owner, source);
}
UnkResult_0x18_gdo fn_71011d4fd0(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    return SetComponent_gdo<0x94, 0, 0x11>(owner, source);
}
UnkResult_0x18_gdo fn_71011d50d0(void*, UnkGuiOwner_gdo* owner, UnkValueSource_gdo** source) {
    return SetComponent_gdo<0x94, 1, 0x11>(owner, source);
}
