// GUI::CDisplayObject unit (tools/tu_map.py, the name from reflection): the eight reflected setters of the
// object's position, one coordinate and the mode it is measured by (x or y, modes 0 to 3) each, as
// fn_71011cc65c and its neighbours in src/game/GUI/CDisplayObject.cpp. Each takes the float picked from
// the argument's holder (inline, or behind a pointer for 9 or more entries or kind 1); a NaN is ignored.
// Each returns an empty result of kind 2. Names and layouts are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

extern const f32 g_unk_7101647774;  // the tolerance the scale is compared to 1 with

struct UnkValueSet_gdp {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_gdp {
    void* holder;
    UnkValueSet_gdp* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_gdp {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkGuiOwner_gdp {
    u8 unk_0x0[8];
    UnkGuiDisplayObject* target;
};

// The placement: the two modes and the position, and the scale after them.
struct UnkGuiPlacement_gdp {
    u8 pad_0x0[0x78];
    u32 h_mode;
    u32 v_mode;
    f32 x;
    f32 y;
    u8 pad_0x88[0xc];
    f32 scale_x;
    f32 scale_y;
};

static inline __attribute__((always_inline)) f32 PickArg_gdp(UnkValueSource_gdp* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return *static_cast<f32*>(src->holder);
    return *reinterpret_cast<f32*>(src);
}

// A unit scale (within the tolerance) changes only the position (bit 0), any other also the size
// (bit 4); the first change runs vf_0xa0.
static inline __attribute__((always_inline)) void MarkPlaced_gdp(UnkGuiDisplayObject* self,
                                                                 const UnkGuiPlacement_gdp* p) {
    f32 sx = p->scale_x;
    f32 eps = g_unk_7101647774;
    if (sx - eps > 1.0f || sx + eps < 1.0f || p->scale_y - eps > 1.0f || eps + p->scale_y < 1.0f) {
        if (!self->field_0x184) {
            self->field_0x184 = true;
            self->vf_0xa0();
        }
        self->field_0x183 = true;
        self->field_0x144 |= 0x11;
    } else {
        if (!self->field_0x184) {
            self->field_0x184 = true;
            self->vf_0xa0();
        }
        self->field_0x183 = true;
        self->field_0x144 |= 1;
    }
}

template <bool Y, u32 Mode>
static inline __attribute__((always_inline)) UnkResult_0x18_gdp SetCoord_gdp(UnkGuiOwner_gdp* owner,
                                                                             UnkValueSource_gdp** source) {
    UnkValueSource_gdp* src = *source;
    UnkGuiDisplayObject* target = owner->target;
    f32 v = PickArg_gdp(src);
    if (!__builtin_isnan(v)) {
        UnkGuiPlacement_gdp* p = reinterpret_cast<UnkGuiPlacement_gdp*>(target);
        if (!Y) {
            if (!(p->h_mode == Mode && p->y - p->y == 0.0f && v - p->x == 0.0f)) {
                p->x = v;
                p->h_mode = Mode;
                MarkPlaced_gdp(target, p);
            }
        } else {
            if (!(p->v_mode == Mode && p->x - p->x == 0.0f && v - p->y == 0.0f)) {
                p->y = v;
                p->v_mode = Mode;
                MarkPlaced_gdp(target, p);
            }
        }
    }
    UnkResult_0x18_gdp result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

UnkResult_0x18_gdp fn_71011d41d8(void*, UnkGuiOwner_gdp* owner, UnkValueSource_gdp** source) {
    return SetCoord_gdp<false, 0>(owner, source);
}
UnkResult_0x18_gdp fn_71011d437c(void*, UnkGuiOwner_gdp* owner, UnkValueSource_gdp** source) {
    return SetCoord_gdp<false, 2>(owner, source);
}
UnkResult_0x18_gdp fn_71011d4524(void*, UnkGuiOwner_gdp* owner, UnkValueSource_gdp** source) {
    return SetCoord_gdp<false, 3>(owner, source);
}
UnkResult_0x18_gdp fn_71011d46cc(void*, UnkGuiOwner_gdp* owner, UnkValueSource_gdp** source) {
    return SetCoord_gdp<false, 1>(owner, source);
}
UnkResult_0x18_gdp fn_71011d4870(void*, UnkGuiOwner_gdp* owner, UnkValueSource_gdp** source) {
    return SetCoord_gdp<true, 0>(owner, source);
}
UnkResult_0x18_gdp fn_71011d4a14(void*, UnkGuiOwner_gdp* owner, UnkValueSource_gdp** source) {
    return SetCoord_gdp<true, 2>(owner, source);
}
UnkResult_0x18_gdp fn_71011d4bbc(void*, UnkGuiOwner_gdp* owner, UnkValueSource_gdp** source) {
    return SetCoord_gdp<true, 3>(owner, source);
}
UnkResult_0x18_gdp fn_71011d4d64(void*, UnkGuiOwner_gdp* owner, UnkValueSource_gdp** source) {
    return SetCoord_gdp<true, 1>(owner, source);
}
