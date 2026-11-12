// GUI::CSprite unit (tools/tu_map.py, the name from reflection): reflected property setters. Each takes
// the value picked from the argument's holder (inline, or behind a pointer for 9 or more entries or kind
// 1). The size setters keep the cell count (+0x1d4, +0x1d8) and the cell size (+0x1a8, +0x1ac), hand
// the product to fn_71011cf8d0 and tell the sprite (vf_0x88); fn_71011fa63c stores a word at +0x1bc and,
// when the sprite has a render object (+0x190), in that object's node too. Each returns an empty result of
// kind 2. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkTU2747.h"
#include "types.h"

struct UnkValueSet_spr {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_spr {
    void* holder;
    UnkValueSet_spr* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_spr {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkSpriteNode_spr {
    u8 unk_0x0[0x88];
    u32 field_0x88;
};

struct UnkSpriteRender_spr {
    u8 unk_0x0[0x178];
    UnkSpriteNode_spr* node;
};

struct UnkSprite_spr {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();

    u8 unk_0x8[0x188];
    UnkSpriteRender_spr* render;  // 0x190
    u8 unk_0x198[0x10];
    f32 cell_w;  // 0x1a8
    f32 cell_h;  // 0x1ac
    u8 unk_0x1b0[0xc];
    u32 field_0x1bc;
    u8 unk_0x1c0[0x14];
    u32 cols;  // 0x1d4
    u32 rows;  // 0x1d8
};

struct UnkSpriteOwner_spr {
    u8 unk_0x0[8];
    UnkSprite_spr* target;
};

template <class T>
static inline __attribute__((always_inline)) T PickArg_spr(UnkValueSource_spr* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return *static_cast<T*>(src->holder);
    return *reinterpret_cast<T*>(src);
}

// Hands the sprite its size, the cell size times the cell count, and tells it.
static inline __attribute__((always_inline)) void UpdateSize_spr(UnkSprite_spr* self) {
    f32 count_x = static_cast<f32>(self->cols);
    f32 count_y = static_cast<f32>(self->rows);
    f32 w = self->cell_w * count_x;
    f32 h = self->cell_h * count_y;
    base::math::CVector2D size = {w, h};
    fn_71011cf8d0(self, &size);
    self->vf_0x88();
}

// Sets the cell size and hands the sprite its new size.
static inline __attribute__((always_inline)) void SetCell_spr(UnkSprite_spr* self, f32 w, f32 h) {
    self->cell_w = w;
    self->cell_h = h;
    f32 cell_w = self->cell_w;
    f32 cell_h = self->cell_h;
    f32 count_x = static_cast<f32>(self->cols);
    f32 count_y = static_cast<f32>(self->rows);
    f32 size_x = cell_w * count_x;
    f32 size_y = cell_h * count_y;
    base::math::CVector2D size = {size_x, size_y};
    fn_71011cf8d0(self, &size);
    self->vf_0x88();
}

static inline __attribute__((always_inline)) UnkResult_0x18_spr Done_spr() {
    UnkResult_0x18_spr result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

UnkResult_0x18_spr fn_71011fe074(void*, UnkSpriteOwner_spr* owner, UnkValueSource_spr** source) {
    UnkValueSource_spr* src = *source;
    UnkSprite_spr* target = owner->target;
    u32 cols = PickArg_spr<u32>(src);
    target->cols = cols;
    UpdateSize_spr(target);
    return Done_spr();
}

UnkResult_0x18_spr fn_71011fe160(void*, UnkSpriteOwner_spr* owner, UnkValueSource_spr** source) {
    UnkValueSource_spr* src = *source;
    UnkSprite_spr* target = owner->target;
    u32 rows = PickArg_spr<u32>(src);
    target->rows = rows;
    UpdateSize_spr(target);
    return Done_spr();
}

UnkResult_0x18_spr fn_71011fe24c(void*, UnkSpriteOwner_spr* owner, UnkValueSource_spr** source) {
    UnkValueSource_spr* src = *source;
    UnkSprite_spr* target = owner->target;
    f32 w = PickArg_spr<f32>(src);
    SetCell_spr(target, w, target->cell_h);
    return Done_spr();
}

UnkResult_0x18_spr fn_71011fe338(void*, UnkSpriteOwner_spr* owner, UnkValueSource_spr** source) {
    UnkValueSource_spr* src = *source;
    UnkSprite_spr* target = owner->target;
    f32 h = PickArg_spr<f32>(src);
    SetCell_spr(target, target->cell_w, h);
    return Done_spr();
}

UnkResult_0x18_spr fn_71011fa63c(void*, UnkSpriteOwner_spr* owner, UnkValueSource_spr** source) {
    UnkValueSource_spr* src = *source;
    UnkSprite_spr* target = owner->target;
    u32 value = PickArg_spr<u32>(src);
    target->field_0x1bc = value;
    if (target->render)
        target->render->node->field_0x88 = value;
    return Done_spr();
}
