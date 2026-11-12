// CTutoEnterLogicAction unit (tools/tu_map.py): fn_7100a59074 calls the script function the id at +0x60
// names, when it is set, with the object at +8 as its argument; the id's use count (+0x44 of its
// string) goes up first. Returns 1. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "game/lua_lib.h"
#include "types.h"

struct lua_State;

// The id's string as this caller sees it.
struct UnkScriptName_b48 {
    u8 pad_0x0[0x10];
    const char* str;
    u8 pad_0x18[0x2c];
    union {
        u32 uses;
    };
};

struct UnkTutoEnterAction_b48 {
    u8 pad_0x0[0x8];
    void* object;
    u8 pad_0x10[0x50];
    UnkScriptName_b48* name;

    // The name's string ("" for none), counting the use.
    const char* NameStr() {
        ++name->uses;
        const char* str = name->str;
        return str ? str : "";
    }
};

u32 fn_7100a59074(UnkTutoEnterAction_b48* self) {
    if (reinterpret_cast<void*>(self->name) != &g_unk_7101d47108->empty) {
        if (fn_71010c0bb0(g_unk_7172a01d18, self->NameStr())) {
            fn_7100fbc4e0(g_unk_7172a01d18, self->object);
            char out[0x100];
            fn_71010c0d80(g_unk_7172a01d18, 1, out, 0x100, nullptr, nullptr);
        }
    }
    return 1;
}
