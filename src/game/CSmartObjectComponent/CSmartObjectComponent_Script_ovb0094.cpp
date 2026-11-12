// CSmartObjectComponent's unit (tools/tu_map.py): reading the object's name from a stream
// (fn_7100a04210) and running its script function with the object and the player before handing the
// player a grab request (fn_7100a042e0; the shape of fn_7100a04ae0 in CSmartObjectComponent_b48.cpp).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CGrabComponent.h"
#include "game/CSmartObjectComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU56.h"
#include "game/lua_lib.h"
#include "types.h"

using base::global::CStrId;

struct UnkPlayer;
void fn_71010d98bc(void* list, void* object);

struct UnkSmartPlayer_ovb0094 {
    u8 unk_0x0[0x200];
    void* field_0x200;
};

struct UnkSmartObject_ovb0094 {
    void** vtable;
    u8 unk_0x8[0x28];
    void* object;  // 0x30

    void SetName(const CStrId& name) {
        reinterpret_cast<void (*)(UnkSmartObject_ovb0094*, const CStrId&)>(vtable[0x168 / 8])(this, name);
    }
};

// Reads a name from the stream ("s"), hands it to the object (vf_0x168) and the object to the player's
// list at +0x200.
bool fn_7100a04210(UnkSmartObject_ovb0094* self, void* stream) {
    char name[0x100];
    fn_710007cf30(stream, "s", name);
    self->SetName(CStrId(name, true));
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310))
        fn_71010d98bc(reinterpret_cast<UnkSmartPlayer_ovb0094*>(player)->field_0x200, self->object);
    return true;
}

struct lua_State;
struct UnkKraidGrab_b57a4;

// A string id's pooled data as these callers see it: the hash at +0x30.
struct UnkSmartIdData_ovb0094 {
    u8 unk_0x0[0x30];
    u64 hash;
};

// The id's string, with a count of its reads at +0x44.
struct UnkSmartScriptName_ovb0094 {
    u8 unk_0x0[0x10];
    const char* str;
    u8 unk_0x18[0x2c];
    union {
        u32 uses;
    };
};

struct UnkSmartLink_ovb0094 {
    u8 unk_0x0[0x10];
    void* entity;
};

struct UnkSmartPlayer2_ovb0094 {
    u8 unk_0x0[0x220];
    void* field_0x220;
    u8 unk_0x228[0x38];
    void* grabber;  // 0x260
};

// The grab request handed to the player's +0x260 (fn_710113d940).
struct UnkSmartGrab_ovb0094 {
    u8 unk_0x0[0x10];
    CStrId name;
    u8 unk_0x18[0xc];
    u32 field_0x24;
    u8 unk_0x28[3];
    bool field_0x2b;
};

struct UnkSmartObject2_ovb0094 {
    u8 unk_0x0[0x30];
    void* object;  // 0x30
    u8 unk_0x38[0x8];
    u32 field_0x40;
    u8 unk_0x44[0x4];
    UnkSmartScriptName_ovb0094* name;  // 0x48
    u8 unk_0x50[0x60];
    UnkSmartLink_ovb0094* link;  // 0xb0
    u8 unk_0xb8[0x8];
    UnkSmartGrab_ovb0094 grab;  // 0xc0

    // The name's string ("" for none), counting the use.
    const char* NameStr() {
        ++name->uses;
        const char* str = name->str;
        return str ? str : "";
    }
};
static_assert(__builtin_offsetof(UnkSmartObject2_ovb0094, grab) == 0xc0, "UnkSmartObject2_ovb0094 layout");

// Calls the script function the id at +0x48 names with the object and the player, then hands the
// player a grab request named by `id`.
void fn_7100a042e0(UnkSmartObject2_ovb0094* self, const CStrId& id) {
    if (!reinterpret_cast<UnkSmartIdData_ovb0094*>(id.data)->hash)
        return;
    auto* player = reinterpret_cast<UnkSmartPlayer2_ovb0094*>(fn_7100dec788(g_unk_7101d47310));
    if (!player) {
        fn_7100a03118(self);
        return;
    }
    if (reinterpret_cast<void*>(self->name) != &g_unk_7101d47108->empty) {
        if (fn_71010c0bb0(g_unk_7172a01d18, self->NameStr())) {
            fn_7100fbc4e0(g_unk_7172a01d18, self->object);
            fn_7100fbc4e0(g_unk_7172a01d18, player);
            char out[0x100];
            fn_71010c0d80(g_unk_7172a01d18, 2, out, 0x100, nullptr, nullptr);
        }
    }
    fn_7100a03118(self);
    if (player->field_0x220 && self->link && self->link->entity && player->grabber) {
        self->grab.field_0x24 = self->field_0x40;
        self->grab.name = id;
        // Stored as a plain byte: the original reloads the grabber afterwards.
        *reinterpret_cast<char*>(&self->grab.field_0x2b) = false;
        fn_710113d940(player->grabber, reinterpret_cast<UnkKraidGrab_b57a4*>(&self->grab),
                      self->link ? self->link->entity : nullptr);
    }
}
