#pragma once

// CDrop (reflected, 0x4f0 bytes, base CGameObject; tools/tu_map.py unit 1222), as far as the hand-written
// sources know it: one layout, grown by each source that reads a member it does not have yet. Reflection
// registers none of its members (tools/gen_fields.py --class CDrop), so the names and the layout are
// placeholders. The dropped-item manager (unit 2562) keeps its drops in arrays and fills each from a table
// of entries built on the stack.
#include "base/color/CColor4F.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

// The dropped-item manager and the item a drop's callback is handed (unit 2562).
struct UnkDroppedItemManager;
struct UnkDroppedItem;

// One entry of a drop's table (0x48 bytes): two ids, two colours and its counts.
struct UnkDropTableEntry {
    base::global::CStrId id = g_unk_7101d470e0;  // 0x0
    base::global::CStrId field_0x8 = g_unk_7101d470e0;
    base::color::CColor4F color{1.0f, 1.0f, 1.0f, 1.0f};  // 0x10
    base::color::CColor4F field_0x20{0.0f, 0.0f, 0.0f, 0.0f};
    u32 field_0x30 = 1;
    f32 field_0x34 = -1.0f;
    s32 field_0x38 = -1;
    s32 field_0x3c = -1;
    bool field_0x40 = false;
};
static_assert(sizeof(UnkDropTableEntry) == 0x48, "placeholder layout");

// A drop's table: how many entries are in use and 15 entries. Its constructor is out of line
// (src/game/CDrop/fn_71007bf550.cpp). Its assignment, fn_7100e2c3f4, and the entry's, fn_7100e2f28c, are
// still written over a view of their own (src/game/UnkTU2562/UnkTU2562_Billboard_ovb0208.cpp): as defaulted
// assignments of these types they match only once base::color::CColor4F declares its assignment as
// fn_7100030728.
struct UnkDropTable {
    u32 count;
    UnkDropTableEntry entries[15];  // 0x8

    UnkDropTable() asm("fn_71007bf550");
};
static_assert(sizeof(UnkDropTable) == 0x440, "placeholder layout");

// An id/path resource record (0x18 bytes), released with fn_7100026f70. It is the record
// base/UnkStrIdPathPair.h calls UnkStrIdPathResource (fn_71000267f8 points one at a path; the manager's
// records at 0xa020 are built as this type by fn_7100e211cc and loaded as that one by fn_7100e21b8c);
// the two are not one type yet because UnkStrIdPathResource has no constructor.
struct UnkDropResource {
    base::global::CStrId id;
    base::global::CFilePathStrId path;
    u64 field_0x10;
    UnkDropResource() : id(g_unk_7101d470e0), path(g_unk_7101d470e8), field_0x10(0) {}
};
static_assert(sizeof(UnkDropResource) == 0x18, "placeholder layout");

struct CDrop {
    // CGameObject
    void** vtable;
    u64 field_0x8;
    // the drop's own members
    void* owner;  // 0x10
    // Called with the owner, the item and the manager's two arguments; fn_7100e2f98c, fn_7100e2f994 and
    // fn_7100e2f99c pass the flag on, fn_7100e2f9a4 and fn_7100e2fa00 ignore it.
    void (*callback)(UnkDroppedItemManager* owner, UnkDroppedItem* item, void* arg, bool flag);  // 0x18
    UnkDropTable table;       // 0x20
    base::global::CStrId item;  // 0x460
    u32 field_0x468;
    base::global::CStrId field_0x470;
    u64 field_0x478;
    u64 field_0x480;
    f32 amount;  // 0x488
    u32 field_0x48c;
    u32 field_0x490;
    u32 field_0x494;
    u32 field_0x498;
    // 0x49c, one byte: the constructor (fn_71007bf3f0) stores it whole (mode 1, the rest clear),
    // fn_7100e2a734 and fn_7100e2a034 set only the low two bits (mode 2) and fn_71007c02dc passes the byte on whole.
    u8 mode : 2;
    u8 field_0x49c_2 : 6;
    UnkDropResource field_0x4a0;
    UnkDropResource field_0x4b8;
    UnkDropResource field_0x4d0;
    bool field_0x4e8;

    // Vtable slot 0x38, called once the drop is filled in.
    void Start() { reinterpret_cast<void (*)(CDrop*)>(vtable[0x38 / 8])(this); }
};
static_assert(__builtin_offsetof(CDrop, item) == 0x460, "placeholder layout");
static_assert(__builtin_offsetof(CDrop, amount) == 0x488, "placeholder layout");
static_assert(__builtin_offsetof(CDrop, field_0x4d0) == 0x4d0, "placeholder layout");
static_assert(sizeof(CDrop) == 0x4f0, "placeholder layout");
