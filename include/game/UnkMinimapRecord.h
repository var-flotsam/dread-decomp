#pragma once

// A 0x50-byte record of the minimap manager's unit (CMinimapManager::CTunableMinimapManager's unit,
// tools/tu_map.py): a name, a small dictionary of 0x20-byte items that each start with a string id,
// and eight words. The records are the values of a CRntSmallDictionary with 4-byte keys (its grow,
// fn_7100ea9c1c, moves them and destroys the old ones) and the last member of the 0x90-byte groups
// that fn_7100e97c60 grows and fn_7100eb3648 clears. Names and layouts are placeholders.

#include "base/UnkDestructors.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "types.h"

// The key type of a record's items: only its array pointer is moved and freed.
struct UnkMinimapRecordItemKey;

struct UnkMinimapRecordItem {
    base::global::CStrId name;  // 0x0
    u8 unk_0x8[0x18];
};
static_assert(sizeof(UnkMinimapRecordItem) == 0x20, "UnkMinimapRecordItem layout");

struct UnkMinimapRecord {
    base::UnkStrIdMember name;  // 0x0
    base::global::CRntSmallDictionary<UnkMinimapRecordItemKey, UnkMinimapRecordItem> items;  // 0x8
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;

    UnkMinimapRecord(UnkMinimapRecord&& other)
        : name(static_cast<base::UnkStrIdMember&&>(other.name)), items(static_cast<decltype(items)&&>(other.items)),
          field_0x30(other.field_0x30), field_0x34(other.field_0x34), field_0x38(other.field_0x38),
          field_0x3c(other.field_0x3c), field_0x40(other.field_0x40), field_0x44(other.field_0x44),
          field_0x48(other.field_0x48), field_0x4c(other.field_0x4c) {}
    // Out of line (src/game/UnkTU2566/UnkTU2566_Minimap_ovb0199.cpp).
    ~UnkMinimapRecord() asm("fn_7100ea9eb8");
};
static_assert(sizeof(UnkMinimapRecord) == 0x50, "UnkMinimapRecord layout");
