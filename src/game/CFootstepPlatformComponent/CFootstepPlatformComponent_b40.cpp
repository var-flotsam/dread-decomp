// CFootstepPlatformComponent unit (tools/tu_map.py unit 1255): fn_71007e7b5c runs the base step
// (fn_71010c4230), then, when the component names a script function (the string id at +0xe0 is not
// the empty one), counts a use of that name (+0x44 of its string entry) and, if the script state has
// the function, calls it with the owner (+0x30) as its argument. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CComponent.h"
#include "game/UnkTU2677.h"
#include "types.h"

struct lua_State;
extern lua_State* g_unk_7172a01d18;
s32 fn_71010c0bb0(lua_State* L, const char* name);
int fn_7100fbc4e0(lua_State* L, void* arg);
struct UnkHandleOwner_71010c4230;

// A string entry of the pool: the text at +0x10, a use count at +0x44.
struct UnkStringEntry_fp {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x2c];
    // A union member (no type-based alias info): the original reloads the id after the increment.
    union {
        u32 uses;
    };
};

struct UnkFootstepPlatform_fp {
    u8 pad_0x0[0x30];
    void* owner;
    u8 pad_0x38[0xa8];
    UnkStringEntry_fp* function;  // 0xe0, a string id
};

void fn_71007e7b5c(UnkFootstepPlatform_fp* self) {
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
    UnkStringEntry_fp* name = self->function;
    if (name == reinterpret_cast<UnkStringEntry_fp*>(&g_unk_7101d47108->empty))
        return;
    lua_State* L = g_unk_7172a01d18;
    ++name->uses;
    const char* text = self->function->text;
    if (fn_71010c0bb0(L, text ? text : "") != 0) {
        fn_7100fbc4e0(g_unk_7172a01d18, self->owner);
        char out[0x100];
        fn_71010c0d80(g_unk_7172a01d18, 1, out, 0x100, nullptr, nullptr);
    }
}
