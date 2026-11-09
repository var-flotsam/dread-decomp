// GUI::CButton's unit (tools/tu_map.py unit 2745, the name from reflection): gaining focus (fn_71011bf558: the sprite
// at +0x278 is tinted 0.6 grey, keeping its alpha, or faded in, or else the focus sound plays unless the parent is a
// GUI::CList, and "OnFocusReceived" is sent), losing it (fn_71011bf660: back to 0.4 grey with the kept alpha, faded
// out, or "OnFocusLost"), a second constructor that ends with vtable slot 0x18 (fn_71011c1410, as fn_71011bf0ec) and
// a forwarder to fn_71011d9e90 or fn_71011cff5c by its last flag (fn_71011c00c0). Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "../../../game_s7/UnkGuiS7.h"
#include "types.h"


struct UnkStateSink;
struct UnkStateKey;
extern UnkStateSink* g_unk_717275c380;
void fn_7100fc9d78(UnkStateSink* sink, UnkStateKey* key, void* state);
struct UnkEnumType;
extern UnkEnumType g_unk_71729f0188;
void fn_71011f9878(UnkGuiDisplayObject* sprite);

struct UnkGuiButtonFocus_ovb0000;
void fn_71011cf630(UnkGuiButtonFocus_ovb0000* self);
void fn_71011d9ab4(UnkGuiButtonFocus_ovb0000* self);

struct UnkButtonColor_ovb0000 {
    f32 r, g, b, a;
    UnkButtonColor_ovb0000(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
};

struct UnkGuiButtonFocus_ovb0000 {
    void** vtable;
    u8 pad_0x8[0x118];
    UnkGuiDisplayObject* parent;  // 0x120
    u8 pad_0x128[0x128];
    bool tint_on_focus;  // 0x250
    bool fade_on_focus;  // 0x251
    u8 pad_0x252[0x26];
    UnkGuiDisplayObject* sprite;  // 0x278
    u8 pad_0x280[0xc];
    f32 alpha;  // 0x28c

    void vf_0x1e8(const char* event, bool flag) {
        reinterpret_cast<void (*)(UnkGuiButtonFocus_ovb0000*, const char*, bool)>(vtable[0x1e8 / 8])(this, event,
                                                                                                   flag);
    }
    void SetColor(const UnkButtonColor_ovb0000& color) {
        unk_gui::fn_71011cfc24(sprite, reinterpret_cast<const unk_gui::UnkColor&>(color), true);
        fn_71011f9878(sprite);
    }
    void SetAlpha(f32 value) {
        unk_gui::fn_71011cfba8(sprite, value);
        fn_71011f9878(sprite);
    }
};
static_assert(__builtin_offsetof(UnkGuiButtonFocus_ovb0000, alpha) == 0x28c, "layout");

void fn_71011bf558(UnkGuiButtonFocus_ovb0000* self) {
    fn_71011d9ab4(self);
    if (self->tint_on_focus) {
        self->alpha = *reinterpret_cast<f32*>(reinterpret_cast<char*>(self->sprite) + 0xac);
        UnkButtonColor_ovb0000 color(0.6f, 0.6f, 0.6f, 1.0f);
        self->SetColor(color);
    } else if (self->fade_on_focus) {
        self->SetAlpha(1.0f);
    } else {
        UnkGuiDisplayObject* parent = self->parent;
        if (!parent || !fn_710009d50c(parent->GetClass(), fn_71011fe904()))
            fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0188), self);
        self->vf_0x1e8("OnFocusReceived", false);
    }
}

void fn_71011bf660(UnkGuiButtonFocus_ovb0000* self) {
    fn_71011cf630(self);
    if (self->tint_on_focus) {
        UnkGuiDisplayObject* sprite = self->sprite;
        UnkButtonColor_ovb0000 color(0.4f, 0.4f, 0.4f, self->alpha);
        unk_gui::fn_71011cfc24(sprite, reinterpret_cast<const unk_gui::UnkColor&>(color), true);
        fn_71011f9878(self->sprite);
    } else if (self->fade_on_focus) {
        self->SetAlpha(0.0f);
    } else {
        self->vf_0x1e8("OnFocusLost", false);
    }
}

using base::global::CStrId;

extern char g_unk_7101cb5908[];  // CButton's vtable
void fn_71011c14e0();            // the bound callback

struct UnkGuiButtonInit_ovb0000 {
    char* vtable;
    u8 base_0x8[0x228];
    CStrId field_0x230;
    CStrId field_0x238;
    CStrId field_0x240;
    CStrId field_0x248;
    u16 field_0x250;
    u8 pad_0x252[0x6];
    void* field_0x258;  // a callback: unused word, the object, the function
    void* object;
    void (*fn)();
    u64 field_0x270;
    u64 field_0x278;
    u64 field_0x280;
    bool field_0x288;
    u8 pad_0x289[0x3];
    u8 field_0x28c[0x15];  // cleared

    void vf_0x18() { reinterpret_cast<void (*)(UnkGuiButtonInit_ovb0000*)>(
        reinterpret_cast<void**>(vtable)[0x18 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkGuiButtonInit_ovb0000, field_0x28c) == 0x28c, "layout");

void fn_71011c1410(UnkGuiButtonInit_ovb0000* self) {
    fn_71011d8d24(self);
    self->vtable = g_unk_7101cb5908 + 0x10;
    new (&self->field_0x230) CStrId("", true);
    new (&self->field_0x238) CStrId("", true);
    new (&self->field_0x240) CStrId("", true);
    new (&self->field_0x248) CStrId("", true);
    self->field_0x250 = 0;
    self->field_0x258 = nullptr;
    self->object = self;
    self->fn = fn_71011c14e0;
    self->field_0x270 = 0;
    self->field_0x278 = 0;
    self->field_0x280 = 0;
    self->field_0x288 = true;
    __builtin_memset(self->field_0x28c, 0, sizeof(self->field_0x28c));
    self->vf_0x18();
}

// As the menu entry item renderers forward (src/game/GUI/UnkMenuEntryItemRenderer_Draw_b65a6.cpp).
void fn_71011d9e90(void* a, void* b, void* c, bool d, bool e, bool f);
void fn_71011cff5c(void* a, void* b, void* c, bool d, bool e, bool f);

void fn_71011c00c0(void* a, void* b, void* c, bool d, bool e, bool f) {
    if (f)
        fn_71011d9e90(a, b, c, d, e, true);
    else
        fn_71011cff5c(a, b, c, d, e, false);
}
