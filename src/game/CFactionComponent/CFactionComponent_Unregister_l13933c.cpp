// CFactionComponent's unit: takes the component out of the faction lists, the counterpart of fn_710113915c
// (CFactionComponent_Register_ovb0139.cpp). Unless the game manager's check (fn_7100deabe4) holds and for a kind
// (+0x70) from 1 to 7, it is removed from its own faction's list, then (as the original has it) never from the list of all.
// Names and layouts are placeholders.
#include "game/CFactionComponent.h"
#include "game/UnkFaction_ovb0139.h"
#include "game/UnkGameManager.h"
#include "types.h"

using UnkFactionNode_l13933c = UnkFactionList_ovb0139::Node;

static inline s32 IndexOf(UnkFactionList_ovb0139* list, UnkFaction_ovb0139* obj) {
    u32 i = 0;
    for (UnkFactionNode_l13933c* n = list->head; n; n = n->next, ++i)
        if (n->value == reinterpret_cast<u64>(obj))
            return i;
    return -1;
}

static inline UnkFactionNode_l13933c* Find(UnkFactionList_ovb0139* list, UnkFaction_ovb0139* obj) {
    for (UnkFactionNode_l13933c* n = list->head; n; n = n->next) {
        if (n->value == reinterpret_cast<u64>(obj))
            return n;
    }
    return nullptr;
}

static inline void Erase(UnkFactionList_ovb0139* list, UnkFactionNode_l13933c* node) {
    UnkFactionNode_l13933c* prev = nullptr;
    UnkFactionNode_l13933c* it = list->head;
    while (it) {
        UnkFactionNode_l13933c* cur = it;
        UnkFactionNode_l13933c* next = it->next;
        it = next;
        if (cur != node) {
            prev = cur;
            continue;
        }
        if (prev) {
            prev->next = next;
            if (list->tail == node)
                list->tail = prev;
        } else {
            list->head = next;
        }
        if (--list->count == 0) {
            list->head = nullptr;
            list->tail = nullptr;
        }
        list->allocator->vf_0x48(node);
        return;
    }
}

static inline void RemoveIfListed(UnkFactionList_ovb0139* list, UnkFaction_ovb0139* obj) {
    if (IndexOf(list, obj) >= 0) {
        if (UnkFactionNode_l13933c* node = Find(list, obj))
            Erase(list, node);
    }
}

void fn_710113933c(UnkFaction_ovb0139* self) {
    if (fn_7100deabe4(g_unk_7101d47310))
        return;
    s32 kind = self->kind;
    if (kind == 0 || kind > 7)
        return;
    switch (kind) {
    case 1:
        RemoveIfListed(&g_unk_7172a08ad8, self);
        break;
    case 2:
        RemoveIfListed(&g_unk_7172a08b48, self);
        break;
    case 4:
        RemoveIfListed(&g_unk_7172a08bb8, self);
        break;
    }
    // Faithful to the original: the list of all is searched only when the component is NOT in it, so it
    // never comes out of that list here.
    if (IndexOf(&g_unk_7172a08c28, self) < 0) {
        if (UnkFactionNode_l13933c* node = Find(&g_unk_7172a08c28, self))
            Erase(&g_unk_7172a08c28, node);
    }
}
