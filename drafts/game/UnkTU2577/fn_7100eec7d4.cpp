// Unit 2577 (near CSubAreaManager::CTunableSubAreaManager): collects the entries of the game manager's
// table at +0x1af8, drops those that some group member's id answers with, and, when any are left,
// copies them into the list the table keeps under the fifth sub-area id. Names and layouts are
// placeholders.
#include <string.h>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2577.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkTableEntry_eec7d4;

// The game manager's table (+0x1af8) as this function reads it: its entries, with empty slots.
struct UnkTableView_eec7d4 {
    u8 pad_0x0[0x90];
    UnkTableEntry_eec7d4** entries;  // 0x90
    u8 pad_0x98[0x8];
    u32 entry_count;  // 0xa0
};

// The entries the table answers an id with.
struct UnkEntryList_eec7d4 {
    UnkTableEntry_eec7d4** data;
    u32 count;
};

// The table's lookups, as their definitions declare them.
struct UnkOwner_7100ca7634;
struct UnkObserverGroupOwner_ovb0055;
struct UnkObserverGroupItems_ovb0055;
void* CBreakableTileManager_FindTileGroupGroup(UnkOwner_7100ca7634*, const CStrId*);
UnkObserverGroupItems_ovb0055* CBreakableTileManager_AddTileGroupGroup(UnkObserverGroupOwner_ovb0055*, const CStrId*);

struct UnkGroupMember_eec7d4 {
    u8 pad_0x0[0x58];
    CStrId id;  // 0x58
};

struct UnkGroup_eec7d4 {
    u8 pad_0x0[0x10];
    UnkGroupMember_eec7d4** members;  // 0x10
    u32 member_count;  // 0x18
    u8 pad_0x1c[0x16c];
};
static_assert(sizeof(UnkGroup_eec7d4) == 0x188, "UnkGroup_eec7d4 layout");

struct UnkSubAreaMgr_eec7d4 {
    u8 pad_0x0[0x860];
    UnkGroup_eec7d4* groups;  // 0x860
    u32 group_count;  // 0x868
};

void fn_7100eec7d4(UnkSubAreaMgr_eec7d4* self) {
    UnkGameManagerSub_0x1af8* table = g_unk_7101d47310->field_0x1af8;
    UnkTableView_eec7d4* view = reinterpret_cast<UnkTableView_eec7d4*>(table);

    CRntVector<UnkTableEntry_eec7d4*> entries(0xf);
    UnkTableEntry_eec7d4** last = view->entries + view->entry_count;
    for (UnkTableEntry_eec7d4** it = view->entries; it != last; ++it) {
        if (UnkTableEntry_eec7d4* entry = *it)
            entries.PushBack(entry);
    }

    for (UnkGroup_eec7d4* group = self->groups; group != self->groups + self->group_count; ++group) {
        UnkGroupMember_eec7d4** end = group->members + group->member_count;
        for (UnkGroupMember_eec7d4** member = group->members; member != end; ++member) {
            auto* list = static_cast<UnkEntryList_eec7d4*>(
                CBreakableTileManager_FindTileGroupGroup(reinterpret_cast<UnkOwner_7100ca7634*>(table), &(*member)->id));
            if (!list)
                continue;
            for (UnkTableEntry_eec7d4** it = list->data; it != list->data + list->count; ++it)
                entries.Remove(*it);
        }
    }

    if (entries.count != 0) {
        auto* dst = reinterpret_cast<CRntVector<UnkTableEntry_eec7d4*>*>(
            CBreakableTileManager_AddTileGroupGroup(reinterpret_cast<UnkObserverGroupOwner_ovb0055*>(table),
                          reinterpret_cast<const CStrId*>(&g_unk_71729daa38[5])));
        if (dst != &entries) {
            dst->Clear();
            dst->Reserve(entries.capacity);
            memcpy(dst->data, entries.data, (u64)entries.count * sizeof(UnkTableEntry_eec7d4*));
            dst->count = entries.count;
        }
    }
    entries.Free();
}
