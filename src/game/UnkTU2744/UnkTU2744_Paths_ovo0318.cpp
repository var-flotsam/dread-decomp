// Unit 2744 (tools/tu_map.py; hint gui/scripts/guimanager.lua): the GUI manager's script paths,
// built as "gui/scripts/" + the lower-cased name + an extension (.lua, .bmscp, .bmssk, .bmssh, and
// "gui/scripts/sprites_" + name + .bmsss), the Lua "<name>.Create" call, and normalising a layout by
// the screen size. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/tunable/CTunable.h"
#include "game/UnkGuiLayout_b0209.h"
#include "game/UnkGuiScreen_b0209.h"
#include "game/UnkGuiWidget_b0209.h"
#include "game/UnkScreenSize_b0209.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU57.h"
#include "types.h"

#include "../../game_aca57c/UnkBufferString.h"

struct UnkWeakTarget;
s32 fn_71010757f0(lua_State* L, s32 nargs, char* error, u32 error_size, UnkWeakTarget** out);

static_assert(sizeof(UnkGuiLayout_b0209) == 0x48, "layout");

static_assert(__builtin_offsetof(UnkGuiScreen_b0209, scale_x) == 0xda0, "layout");

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

UnkWeakTarget* fn_71011b5f5c(u32 unused, const char* const* path, const base::global::CRntString* name, void* arg) {
    UnkWeakTarget* result = nullptr;
    base::global::CRntString func(*name);
    CRntString_AppendCString(&func, ".Create");
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
        CRntString_Append(out.Str(), part.Str());
    }
    CRntString_AppendCString(out.Str(), suffix);
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
