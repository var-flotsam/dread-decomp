#pragma once

// The boss rush results screens' GUI setup: compositions looked up through the GUI registry, and
// their children found by name hash and class. Class names come from reflection (GUI::CLabel,
// GUI::CSprite); names and layouts are placeholders.

#include <functional>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"

namespace unk_gui {

using base::global::CRntString;
using base::global::CStrId;

struct UnkClass;
bool fn_710009d50c(const UnkClass* type, const UnkClass* base);
const UnkClass* fn_71011e8dfc();  // GUI::CLabel::refl_GetClass
const UnkClass* fn_71011f6890();  // GUI::CSprite::refl_GetClass

struct UnkGuiObj {
    virtual const UnkClass* GetClass();
    u8 unk_0x8[0x188];
    const char* field_0x190;
};

// A reference-counted weak handle to the looked-up object, released through vf_0x10.
struct UnkGuiHandleObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    UnkGuiObj* target;
};

struct UnkGuiHandle {
    UnkGuiHandleObj* ptr;

    UnkGuiObj* Get() const { return ptr ? ptr->target : nullptr; }
    ~UnkGuiHandle() {
        if (UnkGuiHandleObj* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkGuiRoot {
    u8 unk_0x0[0x68];
    void* menu;
};

// A text string built from a number.
struct UnkText {
    u8 unk_0x0[0x20];

    ~UnkText();  // fn_7100066e44
};

// Times per boss id (the layout of base::UnkKeyedValueLookup).
struct UnkTimes {
    f32* values;
    s32* keys;
    u32 count;

    f32* End() { return &values[count]; }
    f32* Find(s32 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
};

struct UnkStats {
    u8 unk_0x0[0xb8];
    bool field_0xb8;
    bool field_0xb9;
    u8 unk_0xba[0x16];
    s32 field_0xd0;
    u8 unk_0xd4[0x2c];
    s32 field_0x100;
    u8 unk_0x104[0x934];
    f64 field_0xa38;
    u8 unk_0xa40[0x1f0];
    UnkTimes times;
    u8 unk_0xc48[0x18];
    s32 field_0xc60;
    f32 field_0xc64;
    bool field_0xc68;
};

struct UnkMode {
    u8 unk_0x0[0x550];
    bool survival;
};

struct UnkOwner {
    u8 unk_0x0[0x8];
    UnkMode* mode;
};

extern UnkGuiRoot* g_unk_717271e8f8;
extern UnkStats* g_unk_71726bc570;
extern CStrId g_unk_7172769ca0[2] __attribute__((visibility("hidden")));  // background ids

UnkGuiHandle fn_71011b69d8(UnkGuiRoot* root, const CRntString& name);
UnkGuiObj* fn_71011d98ec(UnkGuiObj* composition, u64 id);  // finds a child
UnkGuiObj* fn_71011d9508(UnkGuiObj* self, const char* const* path);
void fn_71011d968c(void* menu, UnkGuiObj* composition);
void fn_71011b6bbc(UnkGuiRoot* root, UnkGuiObj* object, bool value);
void fn_71011cdce8(UnkGuiObj* object, s32 value);
void fn_71011cf2a4(UnkGuiObj* object, s32 value);
void fn_71011cde44(UnkGuiObj* object, s32 value);
void fn_71011cfba8(UnkGuiObj* object, f32 alpha);
void fn_71011cdc30(UnkGuiObj* object, bool value);
void fn_71011ccb28(UnkGuiObj* object, f32 value);
void fn_71011cc844(UnkGuiObj* object, f32 value);
void fn_71011ec424(UnkGuiObj* label, const CRntString& text);
void fn_71011ec468(UnkGuiObj* label, const UnkText& text, bool value);
void fn_71011ed0f0(UnkGuiObj* label, s32 value);
void fn_71011ed018(UnkGuiObj* label, u32 value);
void fn_71011f8bdc(UnkGuiObj* sprite, CStrId id);
UnkText fn_7100af45b0(s32 value, u32 flags);
void fn_7100eb7358(void* object, u32 a, u32 b);
void fn_7100b95760(UnkGuiObj* button, const s32* id, const std::function<void()>& f,
                   void* unk, u32 flags);
void fn_7100b951f4(UnkGuiObj* button, const s32* id, const std::function<void()>& f,
                   void* unk, u32 flags);

// Copied member by member.
struct UnkColor {
    f32 r, g, b, a;

    UnkColor(const UnkColor& other) : r(other.r), g(other.g), b(other.b), a(other.a) {}
};

struct UnkVec2f {
    f32 x, y;
};

extern const UnkColor g_unk_7101aa00a8;
extern const UnkColor g_unk_7101aa00b8;
extern const UnkColor g_unk_7101aa00c8;

const char32_t* fn_7100af4650(s32 id, u32 flags);
void fn_71011ecfbc(UnkGuiObj* label, const char32_t* text);
void fn_71011ed084(UnkGuiObj* label, u32 value);
void fn_71011cde64(UnkGuiObj* object, const UnkVec2f* value, u32 a, u32 b);
void fn_71011cfc24(UnkGuiObj* object, const UnkColor& color, u32 value);
f32 fn_7100dce130(UnkStats* stats);

// Rounds toward zero.
inline f32 Trunc(f32 t) { return t > 0.0f ? __builtin_floorf(t) : __builtin_ceilf(t); }

// The child of `composition` with this name when it is of the class `get_class` returns, else
// null.
__attribute__((always_inline)) inline UnkGuiObj* FindChild(UnkGuiObj* composition, const char* name,
                                                           const UnkClass* (*get_class)()) {
    UnkGuiObj* child = fn_71011d98ec(composition, fn_7100001570(name, __builtin_strlen(name)));
    return child && fn_710009d50c(child->GetClass(), get_class()) ? child : nullptr;
}

}  // namespace unk_gui
