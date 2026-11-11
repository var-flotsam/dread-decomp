// GUI::CSpriteGrid's unit (tools/tu_map.py): the frame getter as a string id (fn_71011fe3d8), the reset
// (fn_71011fcc54) and the sizing to whole cells (fn_71011fdc78). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiFrames.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflValue;
using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkString_7100fc4b34;

struct UnkSpriteGridFrame_ovb0043 {
    u8 pad_0x0[0x1dc];
    u32 frame;  // 0x1dc
};

// The frame's text (fn_710007eba0), destroyed out of line.
struct UnkFrameString_ovb0043 {
    union {
        CRntString str;
    };
    explicit UnkFrameString_ovb0043(u32 value) { fn_710007eba0(reinterpret_cast<UnkString_7100fc4b34*>(&str), value); }
    ~UnkFrameString_ovb0043() { fn_710007ecd4(&str); }
};

// An id built from a string (fn_7100000850).
struct UnkFrameId_ovb0043 {
    union {
        CStrId id;
    };
    explicit UnkFrameId_ovb0043(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkFrameId_ovb0043() { id.~CStrId(); }
};

// A reflected getter: the grid's frame (+0x1dc) as a string id.
UnkReflValue fn_71011fe3d8(void*, char* obj) {
    auto* grid = *reinterpret_cast<UnkSpriteGridFrame_ovb0043**>(obj + 8);
    UnkFrameId_ovb0043 value = [&] {
        UnkFrameString_ovb0043 text(grid->frame);
        return UnkFrameId_ovb0043(text.str);
    }();
    const base::reflection::CType* type = fn_7100009658();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value.id, false);
    return r;
}

struct UnkSpriteGridReset_ovb0043 {
    void** vtable;
    u8 pad_0x8[0x181];
    bool field_0x189;
    u8 pad_0x18a[0xe];
    CFilePathStrId path;  // 0x198: the texture's
    u64 field_0x1a0;
    u64 field_0x1a8;
    f32 color[4];  // 0x1b0
    u64 field_0x1c0;
    u64 field_0x1c8;
    u64 field_0x1d0;
    u32 field_0x1d8;
    u8 pad_0x1dc[4];
    bool field_0x1e0;
    bool field_0x1e1;
};

// The reset: the display object's (fn_71011cd4b4), then the empty texture path, a white colour and
// cleared members.
void fn_71011fcc54(UnkSpriteGridReset_ovb0043* self) {
    fn_71011cd4b4(self);
    CFilePathStrId* path = &self->path;
    const CFilePathStrId* value = &g_unk_7101d470e8;
    if (path->data != value->data) {
        UnkStringData* d = value->data;
        UnkStringPool* pool = g_unk_7101d47200;
        if (d != &pool->empty)
            ++d->ref_count;
        if (path->data != &pool->empty) {
            --path->data->ref_count;
            if (path->data->ref_count == 0)
                fn_71000803a4(pool, path->data);
        }
        path->data = d;
    }
    self->field_0x1a0 = 0;
    self->field_0x1a8 = 0;
    self->field_0x1c0 = 0;
    self->field_0x1c8 = 0;
    self->color[0] = 1.0f;
    self->color[1] = 1.0f;
    self->color[2] = 1.0f;
    self->color[3] = 1.0f;
    self->field_0x1d0 = 0;
    self->field_0x1d8 = 0;
    self->field_0x1e0 = false;
    self->field_0x1e1 = false;
    self->field_0x189 = true;
}

struct UnkSpriteGridTexture_ovb0043 {
    u8 pad_0x0[0x10];
    u32 width;  // 0x10
    u32 height;  // 0x14
};

struct UnkSpriteGridModel_ovb0043 {
    u8 pad_0x0[0x10];
    f32 scale_x;  // 0x10
    f32 scale_y;  // 0x14
    UnkSpriteGridTexture_ovb0043* texture;  // 0x18
};

struct UnkSpriteGrid_ovb0043 {
    void** vtable;
    u8 pad_0x8[0x188];
    void* field_0x190;
    u8 pad_0x198[0x8];
    UnkSpriteGridModel_ovb0043* model;  // 0x1a0
    u8 pad_0x1a8[0x2c];
    u32 columns;  // 0x1d4
    u32 rows;  // 0x1d8
};

// A size whose assignment copies the two components one by one.
struct UnkGridSize_ovb0043 {
    f32 x;
    f32 y;
    UnkGridSize_ovb0043(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkGridSize_ovb0043(const UnkGridSize_ovb0043& other) : x(other.x), y(other.y) {}
    UnkGridSize_ovb0043& operator=(const UnkGridSize_ovb0043& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
    UnkGridSize_ovb0043& operator=(const UnkLabelVec2& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
};

UnkGridSize_ovb0043 fn_71011cf214(UnkSpriteGrid_ovb0043* self, const UnkGridSize_ovb0043* size);

// The size of one cell (not matched here).
UnkGridSize_ovb0043 fn_71011fd160(UnkSpriteGrid_ovb0043* self);

struct UnkSprite_spr;
void fn_71011cf8d0(UnkSprite_spr* self, const base::math::CVector2D* size);

// Sizes the grid to its columns (+0x1d4) and rows (+0x1d8) of cells (fn_71011fd160).
void fn_71011fdc78(UnkSpriteGrid_ovb0043* self) {
    if (!self->columns || !self->rows)
        return;
    base::math::CVector2D size;
    UnkGridSize_ovb0043 cell = fn_71011fd160(self);
    f32 columns = (f32)self->columns;
    f32 rows = (f32)self->rows;
    size = {cell.x * columns, cell.y * rows};
    fn_71011cf8d0(reinterpret_cast<UnkSprite_spr*>(self), &size);
}
