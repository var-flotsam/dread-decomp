// CFactionComponent's unit (tools/tu_map.py): registers the component in the faction lists. Unless the game manager's check
// (fn_7100deabe4) holds and for a kind (+0x70) from 1 to 7, it is appended to the list of its own faction (1: g_unk_7172a08ad8,
// 2: g_unk_7172a08b48, 4: g_unk_7172a08bb8) unless already listed, then likewise to the list of all (g_unk_7172a08c28).
// Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkFactionListTag_ovb0139;
using UnkFactionList_ovb0139 = base::UnkSList<UnkFactionListTag_ovb0139>;

extern UnkFactionList_ovb0139 g_unk_7172a08ad8;  // players
extern UnkFactionList_ovb0139 g_unk_7172a08b48;  // neutral
extern UnkFactionList_ovb0139 g_unk_7172a08bb8;  // enemies
extern UnkFactionList_ovb0139 g_unk_7172a08c28;  // all

struct UnkFaction_ovb0139 {
    u8 pad_0x0[0x70];
    s32 kind;  // 0x70
};

static inline s32 IndexOf(UnkFactionList_ovb0139* list, UnkFaction_ovb0139* obj) {
    u32 i = 0;
    for (UnkFactionList_ovb0139::Node* n = list->head; n; n = n->next, ++i)
        if (n->value == reinterpret_cast<u64>(obj))
            return i;
    return -1;
}

static inline void AddUnique(UnkFactionList_ovb0139* list, UnkFaction_ovb0139* obj) {
    if (IndexOf(list, obj) < 0)
        list->PushBack(reinterpret_cast<u64>(obj));
}

void fn_710113915c(UnkFaction_ovb0139* self) {
    if (fn_7100deabe4(g_unk_7101d47310))
        return;
    s32 kind = self->kind;
    if (kind == 0 || kind > 7)
        return;
    switch (kind) {
    case 1:
        AddUnique(&g_unk_7172a08ad8, self);
        break;
    case 2:
        AddUnique(&g_unk_7172a08b48, self);
        break;
    case 4:
        AddUnique(&g_unk_7172a08bb8, self);
        break;
    }
    AddUnique(&g_unk_7172a08c28, self);
}
