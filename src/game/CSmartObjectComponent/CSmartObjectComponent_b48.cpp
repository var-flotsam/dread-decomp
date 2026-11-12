// CSmartObjectComponent unit (tools/tu_map.py): fn_7100a04ae0 calls the script function the id at +0x48
// names, when there is a player (fn_7100dec788) and the id is set, with three arguments: the object at
// +0x30, the player and the entity of the link at +0xb0 (null without one). The id's use count (+0x44
// of its string) goes up as its string is read (the shape of CTutoEnterLogicAction_b48.cpp). Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "game/lua_lib.h"
#include "types.h"

struct lua_State;

struct UnkPlayer;

// The id's string as this caller sees it.
struct UnkScriptName_a04ae0 {
    u8 pad_0x0[0x10];
    const char* str;
    u8 pad_0x18[0x2c];
    union {
        u32 uses;
    };
};

struct UnkSmartObjectLink_b48 {
    u8 pad_0x0[0x10];
    void* entity;
};

struct UnkSmartObjectComponent_b48 {
    u8 pad_0x0[0x30];
    void* object;
    u8 pad_0x38[0x10];
    UnkScriptName_a04ae0* name;
    u8 pad_0x50[0x60];
    UnkSmartObjectLink_b48* link;

    // The name's string ("" for none), counting the use.
    const char* NameStr() {
        ++name->uses;
        const char* str = name->str;
        return str ? str : "";
    }
};

void fn_7100a04ae0(UnkSmartObjectComponent_b48* self) {
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (!player)
        return;
    if (reinterpret_cast<void*>(self->name) != &g_unk_7101d47108->empty) {
        if (fn_71010c0bb0(g_unk_7172a01d18, self->NameStr())) {
            fn_7100fbc4e0(g_unk_7172a01d18, self->object);
            fn_7100fbc4e0(g_unk_7172a01d18, player);
            fn_7100fbc4e0(g_unk_7172a01d18, self->link ? self->link->entity : nullptr);
            char out[0x100];
            fn_71010c0d80(g_unk_7172a01d18, 3, out, 0x100, nullptr, nullptr);
        }
    }
}
