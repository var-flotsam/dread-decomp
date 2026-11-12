#pragma once

// The game's table of string ids: one object of 0x2578 bytes, each slot a string id, built on first use by
// fn_7100031894 and read all over the game. Only the slots some hand-written file reads have a member here;
// their real names are not known. A file that reads a slot as something else than a string id (the id's
// data as a key record, a property name) says so with a cast where it does.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkGameManagerIdTable {
    u8 pad_0x0[0x4c8];
    base::global::CStrId field_0x4c8;
    u8 pad_0x4d0[0x5f8 - 0x4d0];
    base::global::CFilePathStrId field_0x5f8;
    u8 pad_0x600[0x628 - 0x600];
    base::global::CStrId field_0x628;
    u8 pad_0x630[0x7c8 - 0x630];
    base::global::CStrId field_0x7c8;
    u8 pad_0x7d0[0x8b8 - 0x7d0];
    base::global::CStrId field_0x8b8;
    u8 pad_0x8c0[0x8e8 - 0x8c0];
    base::global::CStrId field_0x8e8;
    base::global::CStrId field_0x8f0;
    u8 pad_0x8f8[0x980 - 0x8f8];
    base::global::CStrId field_0x980;
    u8 pad_0x988[0x998 - 0x988];
    base::global::CStrId field_0x998;
    u8 pad_0x9a0[0xa00 - 0x9a0];
    base::global::CStrId field_0xa00;
    u8 pad_0xa08[0xb10 - 0xa08];
    base::global::CStrId field_0xb10;
    u8 pad_0xb18[0xd30 - 0xb18];
    base::global::CStrId field_0xd30;
    u8 pad_0xd38[0xd40 - 0xd38];
    base::global::CStrId field_0xd40;
    u8 pad_0xd48[0xd80 - 0xd48];
    base::global::CStrId field_0xd80;
    u8 pad_0xd88[0xd90 - 0xd88];
    base::global::CStrId field_0xd90;
    u8 pad_0xd98[0xec8 - 0xd98];
    base::global::CStrId field_0xec8;
    u8 pad_0xed0[0xf18 - 0xed0];
    base::global::CStrId field_0xf18;
    u8 pad_0xf20[0xf80 - 0xf20];
    base::global::CStrId field_0xf80;
    u8 pad_0xf88[0x1148 - 0xf88];
    base::global::CStrId field_0x1148;
    base::global::CStrId field_0x1150;
    u8 pad_0x1158[0x11a8 - 0x1158];
    base::global::CStrId field_0x11a8;
    u8 pad_0x11b0[0x1240 - 0x11b0];
    base::global::CStrId field_0x1240;
    u8 pad_0x1248[0x13f8 - 0x1248];
    base::global::CStrId field_0x13f8;
    u8 pad_0x1400[0x1570 - 0x1400];
    base::global::CStrId field_0x1570;
    u8 pad_0x1578[0x1640 - 0x1578];
    base::global::CStrId field_0x1640;
    u8 pad_0x1648[0x16d8 - 0x1648];
    base::global::CStrId field_0x16d8;
    u8 pad_0x16e0[0x1770 - 0x16e0];
    base::global::CStrId field_0x1770;
    u8 pad_0x1778[0x17a0 - 0x1778];
    base::global::CStrId field_0x17a0;
    u8 pad_0x17a8[0x17e0 - 0x17a8];
    base::global::CStrId field_0x17e0;
    u8 pad_0x17e8[0x1848 - 0x17e8];
    base::global::CStrId field_0x1848;
    u8 pad_0x1850[0x18a0 - 0x1850];
    base::global::CStrId field_0x18a0;
    u8 pad_0x18a8[0x18e8 - 0x18a8];
    base::global::CStrId field_0x18e8;
    base::global::CStrId field_0x18f0;
    u8 pad_0x18f8[0x1a90 - 0x18f8];
    base::global::CStrId field_0x1a90;
    u8 pad_0x1a98[0x1b18 - 0x1a98];
    base::global::CStrId field_0x1b18;
    u8 pad_0x1b20[0x1b40 - 0x1b20];
    base::global::CStrId field_0x1b40;
    u8 pad_0x1b48[0x1c68 - 0x1b48];
    base::global::CStrId field_0x1c68;
    u8 pad_0x1c70[0x1cb0 - 0x1c70];
    base::global::CStrId field_0x1cb0;
    base::global::CStrId field_0x1cb8;
    u8 pad_0x1cc0[0x1d78 - 0x1cc0];
    base::global::CStrId field_0x1d78;
    u8 pad_0x1d80[0x1f00 - 0x1d80];
    base::global::CStrId field_0x1f00;
    u8 pad_0x1f08[0x2050 - 0x1f08];
    base::global::CStrId field_0x2050;
    u8 pad_0x2058[0x20f0 - 0x2058];
    base::global::CStrId field_0x20f0;
    u8 pad_0x20f8[0x22b8 - 0x20f8];
    base::global::CStrId field_0x22b8;
    u8 pad_0x22c0[0x23a8 - 0x22c0];
    base::global::CStrId field_0x23a8;
    u8 pad_0x23b0[0x2418 - 0x23b0];
    base::global::CStrId field_0x2418;
    u8 pad_0x2420[0x2438 - 0x2420];
    base::global::CStrId field_0x2438;
    u8 pad_0x2440[0x2578 - 0x2440];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
static_assert(sizeof(UnkGameManagerIdTable) == 0x2578, "UnkGameManagerIdTable placeholder layout");

// The table, built the first time it is asked for.
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}
