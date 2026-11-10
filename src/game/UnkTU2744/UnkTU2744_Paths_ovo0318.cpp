// Unit 2744 (tools/tu_map.py; hint gui/scripts/guimanager.lua): the GUI manager's script paths,
// built as "gui/scripts/" + the lower-cased name + an extension (.lua, .bmscp, .bmssk, .bmssh, and
// "gui/scripts/sprites_" + name + .bmsss), the Lua "<name>.Create" call, and normalising a layout by
// the screen size. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "types.h"

#include "../../game_aca57c/UnkBufferString.h"

struct UnkWeakTarget;
extern lua_State* g_unk_7172a01d18;
s32 fn_71010c0bb0(lua_State* L, const char* str);
int fn_71010c0b40(lua_State* L, const char* fmt, ...);
s32 fn_71010757f0(lua_State* L, s32 nargs, char* error, u32 error_size, UnkWeakTarget** out);
void fn_710007f564(void* self, const char* str);  // appends a string

u32 fn_7100132fc0();
u32 fn_7100132fd0();

struct UnkGuiWidget_b0209 {
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
    virtual void vf_0x58(f32 value);
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
};

struct UnkGuiLayout_b0209 {
    u8 pad_0x0[8];
    s32 kind;  // 0x8
    u8 pad_0xc[0x18];
    f32 width;  // 0x24
    u8 pad_0x28[4];
    f32 height;  // 0x2c
    u8 pad_0x30[0x18];
};
static_assert(sizeof(UnkGuiLayout_b0209) == 0x48, "layout");

struct UnkGuiScreen_b0209 {
    u8 pad_0x0[0x40];
    UnkGuiLayout_b0209* layouts;  // 0x40
    u8 pad_0x48[8];
    u32 layout_count;  // 0x50
    u8 pad_0x54[0x14];
    UnkGuiWidget_b0209* target;  // 0x68
    u8 pad_0x70[0xd28];
    f32 field_0xd98;
    f32 field_0xd9c;
    f32 scale_x;  // 0xda0
    f32 scale_y;  // 0xda4
};
static_assert(__builtin_offsetof(UnkGuiScreen_b0209, scale_x) == 0xda0, "layout");

struct UnkScreenSize_b0209 {
    f32 width;
    f32 height;
};

static inline UnkScreenSize_b0209 ScreenSize() {
    f32 w = static_cast<f32>(fn_7100132fc0());
    f32 h = static_cast<f32>(fn_7100132fd0());
    return UnkScreenSize_b0209{w, h};
}

// Normalises the second layout (kind 2) by the screen size and refreshes the target.
void fn_71011b350c(UnkGuiScreen_b0209* self) {
    UnkGuiLayout_b0209* layout = nullptr;
    UnkGuiLayout_b0209* end = self->layouts + self->layout_count;
    for (UnkGuiLayout_b0209* p = self->layouts; p != end; ++p) {
        if (p->kind == 2) {
            layout = p;
            break;
        }
    }
    static UnkScreenSize_b0209 screen = ScreenSize();
    f32 lw = layout->width;
    f32 lh = layout->height;
    UnkScreenSize_b0209 size = screen;
    self->field_0xd98 = lw / size.width;
    self->field_0xd9c = lh / size.height;
    if (self->target) {
        self->target->vf_0x88();
        self->target->vf_0x58(0.0f);
        self->target->vf_0x60();
    }
}

UnkWeakTarget* fn_71011b5f5c(void* unused, const char* const* path, const base::global::CRntString* name, void* arg) {
    UnkWeakTarget* result = nullptr;
    base::global::CRntString func(*name);
    fn_710007f564(&func, ".Create");
    if (fn_71010c0bb0(g_unk_7172a01d18, func.data)) {
        s32 nargs;
        if (arg)
            nargs = fn_71010c0b40(g_unk_7172a01d18, "st", *path ? *path : g_unk_710162e2ed, arg);
        else
            nargs = fn_71010c0b40(g_unk_7172a01d18, "s", *path ? *path : g_unk_710162e2ed);
        char error[0x100];
        fn_71010757f0(g_unk_7172a01d18, nargs, error, 0x100, &result);
    }
    return result;
}

using UnkPathText_o0318 = UnkBufferString<0x100, g_unk_7101a194b8>;

// Copies `src` lower-cased (ASCII) into `dst`.
static inline void ToLower(char* dst, const char* src) {
    dst[0] = '\0';
    u32 length = __builtin_strlen(src);
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = src[i];
            dst[i] = (u8)(c - 'A') < 26 ? c + ('a' - 'A') : c;
        }
        dst[length] = '\0';
    }
}

// prefix + lower-cased name + suffix.
static inline __attribute__((always_inline)) UnkPathText_o0318 BuildPath(const char* const* name, const char* prefix,
                                                                         const char* suffix) {
    UnkPathText_o0318 out(prefix);
    char lower[0x100];
    ToLower(lower, *name);
    {
        UnkPathText_o0318 part(lower);
        fn_710007f3b4(out.Str(), part.Str());
    }
    fn_710007f564(out.Str(), suffix);
    return out;
}

UnkPathText_o0318 fn_71011b659c(void* unused, const char* const* name) {
    return BuildPath(name, "gui/scripts/", ".lua");
}

UnkPathText_o0318 fn_71011b66fc(void* unused, const char* const* name) {
    return BuildPath(name, "gui/scripts/", ".bmscp");
}

UnkPathText_o0318 fn_71011b8a80(void* unused, const char* const* name) {
    return BuildPath(name, "gui/scripts/", ".bmssk");
}

UnkPathText_o0318 fn_71011b965c(void* unused, const char* const* name) {
    return BuildPath(name, "gui/scripts/sprites_", ".bmsss");
}

UnkPathText_o0318 fn_71011ba4fc(void* unused, const char* const* name) {
    return BuildPath(name, "gui/scripts/", ".bmssh");
}
