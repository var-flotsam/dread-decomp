// GUI::CSprite's unit (tools/tu_map.py): the sprite's mode by name (the two names of
// g_unk_7101cb6858), its constructor and reset (both use a guarded all-bits mask), showing an image
// of a sheet or none, and sizing the sprite to its image. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/math/CVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/GUI/CSprite.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiFrames.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkGuiSprite.h"
#include "game/UnkTU1722.h"
#include "game/UnkTU254.h"
#include "game/UnkTU2744.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkMinimapView;

// A string id's data as the name compare sees it: the text's key at +0x30 and a use count at +0x44
// (a union member: the original reloads the id after the increment).
struct UnkStrIdText_ovb0076 {
    u8 pad_0x0[0x30];
    s64 key;
    u8 pad_0x38[0xc];
    union {
        s32 field_0x44;
    };
};

struct UnkSpriteRenderObj_ovb0076 {
    u32 flags;  // bits 12..15: the frame it was made in
    u8 pad_0x4[0x174];
    UnkMinimapView* view;  // 0x178
};

// An image the sprite shows: its name id first.
struct UnkSpriteImage_ovb0076 {
    CStrId name;
};

struct UnkSprite_ovb0076 {
    void** vtable;
    u8 pad_0x8[0x13c];
    s32 field_0x144;
    u8 pad_0x148[0x3c];
    bool dirty;  // 0x184
    u8 pad_0x185[0xb];
    UnkSpriteRenderObj_ovb0076* render;  // 0x190
    u8 pad_0x198[0x20];
    u32 mode;         // 0x1b8
    u32 field_0x1bc;
    u8 pad_0x1c0[0x8];
    CStrId sheet;  // 0x1c8
    CStrId name;   // 0x1d0
    UnkSpriteImage_ovb0076* image;  // 0x1d8
    u32 field_0x1e0;
    u32 field_0x1e4;
    bool field_0x1e8;
};
static_assert(__builtin_offsetof(UnkSprite_ovb0076, field_0x1e8) == 0x1e8, "UnkSprite_ovb0076 layout");

static inline __attribute__((always_inline)) bool NameIs_ovb0076(const CStrId& id, const char* name) {
    CRntString str(name);
    UnkStrIdText_ovb0076* text = reinterpret_cast<UnkStrIdText_ovb0076*>(id.data);
    ++text->field_0x44;
    return reinterpret_cast<UnkStrIdText_ovb0076*>(id.data)->key == str.key;
}

// The id's text, counting the use.
static inline __attribute__((always_inline)) const CRntString* Text_ovb0076(const CStrId& id) {
    ++reinterpret_cast<UnkStrIdText_ovb0076*>(id.data)->field_0x44;
    return reinterpret_cast<const CRntString*>(reinterpret_cast<u8*>(id.data) + 0x10);
}

s32 fn_71011f8750(const CStrId& id) {
    if (NameIs_ovb0076(id, g_unk_7101cb6858[0]))
        return 0;
    if (NameIs_ovb0076(id, g_unk_7101cb6858[1]))
        return 1;
    return 0x7fffffff;
}

void fn_71011f8800(UnkSprite_ovb0076* self, const CStrId& id) {
    u32 mode;
    if (NameIs_ovb0076(id, g_unk_7101cb6858[0]))
        mode = 0;
    else if (NameIs_ovb0076(id, g_unk_7101cb6858[1]))
        mode = 1;
    else
        return;
    if (self->mode != mode) {
        self->mode = mode;
        if (self->render)
            fn_71001c3770(self->render->view, mode);
    }
}

inline const u32& AllBits_ovb0076() {
    static u32 s_value = ~base::reflection::refl_GetEnumZero<0x7172a1098c>() & 3;
    return s_value;
}

void fn_71011f8af4(UnkSprite_ovb0076* self) {
    fn_71011cd4b4(self);
    self->mode = 0;
    self->field_0x1bc = 0;
    self->field_0x1e4 = 0;
    self->field_0x1e8 = true;
    fn_71011d0088(reinterpret_cast<UnkGuiDisplayObject*>(self), true);
    self->field_0x1e0 |= AllBits_ovb0076();
}

void fn_71011fa468(UnkSprite_ovb0076* self) {
    self->field_0x144 = -1;
    if (!self->dirty) {
        self->dirty = true;
        reinterpret_cast<void (*)(UnkSprite_ovb0076*)>(self->vtable[0xa0 / 8])(self);
    }
    self->field_0x1e0 = AllBits_ovb0076();
}

void fn_71011b9a1c(UnkGuiRoot* root, const CRntString* sheet);

// Shows an image of a sheet, or none: then the sheet is let go (fn_71011b9a1c) and the render
// object is handed back to the GUI root.
void fn_71011f8f80(UnkSprite_ovb0076* self, UnkSpriteImage_ovb0076* image, CStrId sheet) {
    if (image) {
        self->sheet = sheet;
        CStrId name(image->name);
        self->name.ReleaseAndTake(name);
        self->image = image;
        return;
    }
    self->image = nullptr;
    self->name = g_unk_7101d470e0;
    fn_71011b9a1c(g_unk_717271e8f8, Text_ovb0076(self->sheet));
    self->sheet = g_unk_7101d470e0;
    if (UnkSpriteRenderObj_ovb0076* render = self->render) {
        fn_71011bab80(g_unk_717271e8f8, render, (render->flags >> 12) & 0xf);
        self->render = nullptr;
    }
}

void fn_71011f918c(UnkSprite_ovb0076* self) {
    if (UnkSpriteRenderObj_ovb0076* render = self->render) {
        fn_71011bab80(g_unk_717271e8f8, render, (render->flags >> 12) & 0xf);
        self->render = nullptr;
    }
    fn_71011f8f80(self, nullptr, g_unk_7101d470e0);
}

// The image's size in a frame, and the sprite's size setter (as src/game/GUI/CLabel.cpp and
// src/game/GUI/CSprite/CSprite_PropSetters.cpp declare them).
struct UnkLabelSource;
u32 fn_7100132f80();
u32 fn_7100132f90();
struct UnkSprite_spr;
void fn_71011cf8d0(UnkSprite_spr* self, const base::math::CVector2D* size);

struct UnkSpriteView2_ovb0076 {
    u8 pad_0x0[0x80];
    u32 width;
    u32 height;
};

struct UnkSpriteRender2_ovb0076 {
    u8 pad_0x0[0x178];
    UnkSpriteView2_ovb0076* view;
};

struct UnkSprite2_ovb0076 {
    u8 pad_0x0[0x190];
    UnkSpriteRender2_ovb0076* render;
    u8 pad_0x198[0x40];
    UnkLabelSource* image;  // 0x1d8
};

struct UnkPixelScale_ovb0076 {
    u8 pad_0x0[0x10];
    base::math::CVector2D scale;
};

// Where an image's pixel scale is kept (never set in this unit).
static UnkPixelScale_ovb0076* s_pixel_scale_ovb0076 = nullptr;

// The size of the sprite's image in the frame, or zero.
static inline __attribute__((always_inline)) void ImageSize_ovb0076(UnkSprite2_ovb0076* self, s32 frame,
                                                                    base::math::CVector2D& size) {
    if (frame != 0x7fffffff) {
        if (self->image) {
            UnkLabelVec2 v = fn_7100b82b18(self->image, frame);
            size.x = v.x;
            size.y = v.y;
        } else if (self->render) {
            UnkSpriteView2_ovb0076* view = self->render->view;
            u32 w = view->width;
            u32 h = view->height;
            if (w != 0 && h != 0) {
                f32 fh = (f32)h;
                f32 fw = (f32)w;
                f32 x = s_pixel_scale_ovb0076->scale.x * fw;
                f32 y = s_pixel_scale_ovb0076->scale.y * fh;
                base::math::CVector2D pixels = {x, y};
                UnkLabelVec2 v = fn_71011b7fc8(g_unk_717271e8f8, frame, &pixels);
                size.x = v.x;
                size.y = v.y;
            }
        }
    }
}

// Sizes the sprite to its image in the frame (by the image's own size, or the render object's
// pixel size), scaled up past 1600x900 screens.
void fn_71011fa190(UnkSprite2_ovb0076* self, s32 frame) {
    base::math::CVector2D size = {0.0f, 0.0f};
    ImageSize_ovb0076(self, frame, size);
    if (fn_7100132f80() > 1600)
        size.x *= (f32)fn_7100132f80() / 1600.0f;
    if (fn_7100132f90() > 900)
        size.y *= (f32)fn_7100132f90() / 900.0f;
    fn_71011cf8d0(reinterpret_cast<UnkSprite_spr*>(self), &size);
}
