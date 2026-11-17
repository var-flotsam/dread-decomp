// UnkTU2727 (main menu script unit): the main menu object's constructor (fn_71011902d4): the music
// trigger base (fn_7101186780), the menu's vtable, its three 0x140-byte entries (a property bag, flags
// and a string each), an empty sound handle and the name "MAINMENU". Names and layouts are placeholders.
#include <new>

#include "../../game_s7/UnkPropBagS7.h"
#include "base/global/CRntString.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2725.h"
#include "game/UnkTU57.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CRntString;

struct UnkMusicTrigger;
extern void* g_unk_7101cb51b8[];  // the menu's vtable

// An entry's text: an empty string (fn_710007eba0 with no capacity).
struct UnkMenuText_l1902d4 {
    u8 raw[0x30];
    UnkMenuText_l1902d4() { fn_710007eba0(reinterpret_cast<UnkString_7100fc4b34*>(this), 0); }
};

struct UnkMenuEntry_l1902d4 {
    unk_prop_bag::UnkPropBag bag;  // 0x0
    bool field_0x88 = false;
    u8 pad_0x89[3];
    u8 field_0x8c[0x25] = {};
    u8 pad_0xb1[3];
    u32 field_0xb4 = 0;
    u8 field_0xb8[0x16];
    u8 pad_0xce[2];
    u8 field_0xd0[7] = {};
    bool field_0xd7 = true;
    u64 field_0xd8 = 0;
    u64 field_0xe0 = 0;
    u64 field_0xe8 = 0;
    u16 field_0xf0 = 0;
    u8 pad_0xf2[2];
    u8 field_0xf4[0x14] = {};
    UnkMenuText_l1902d4 text;  // 0x108
    bool field_0x138;

    UnkMenuEntry_l1902d4() {
        __builtin_memset(field_0xb8, 0, sizeof(field_0xb8));
        field_0x138 = false;
    }
};
static_assert(__builtin_offsetof(UnkMenuEntry_l1902d4, field_0xd0) == 0xd0, "layout");
static_assert(__builtin_offsetof(UnkMenuEntry_l1902d4, field_0xf4) == 0xf4, "layout");
static_assert(__builtin_offsetof(UnkMenuEntry_l1902d4, text) == 0x108, "layout");
static_assert(sizeof(UnkMenuEntry_l1902d4) == 0x140, "layout");

struct UnkMainMenu_l1902d4 {
    void** vtable;
    CRntString name;  // 0x8
    u8 pad_0x38[0x18];
    u64 field_0x50;
    u64 field_0x58;
    s32 field_0x60;
    void* field_0x68;
    void* field_0x70;
    UnkMenuEntry_l1902d4 entries[3];  // 0x78
    bool field_0x438;
    UnkSoundHandle sound;  // 0x440
};
static_assert(__builtin_offsetof(UnkMainMenu_l1902d4, entries) == 0x78, "layout");
static_assert(__builtin_offsetof(UnkMainMenu_l1902d4, sound) == 0x440, "layout");

void fn_71011902d4(UnkMainMenu_l1902d4* self) {
    fn_7101186780(reinterpret_cast<UnkMusicTrigger*>(self));
    self->vtable = &g_unk_7101cb51b8[2];
    self->field_0x58 = 0;
    self->field_0x60 = -1;
    self->field_0x68 = nullptr;
    self->field_0x70 = nullptr;
    new (&self->entries[0]) UnkMenuEntry_l1902d4();
    new (&self->entries[1]) UnkMenuEntry_l1902d4();
    new (&self->entries[2]) UnkMenuEntry_l1902d4();
    self->field_0x438 = false;
    self->sound = g_unk_7108b6c460;
    fn_710007e77c(&self->name, "MAINMENU");
    self->field_0x50 = 0;
}
