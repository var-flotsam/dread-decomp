// The CKraidAIComponent unit. fn_71006015bc runs the base handler (fn_710110bb4c), then reorders the other
// object's list (+0x28, base::UnkSList) of the component's entries: the entry at +0x14aa0 moves to the
// front when the entry at +0x14bc0 heads the other's track list (+0x20) ahead of +0x14b80; in state 1
// (+0x1140) the entry at +0x14ad8 moves to the front once three or more leading tracks are +0x14aa0 or
// +0x14a38; otherwise a lone +0x14a38 track moves to the back. Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkKraidListTag_l6015bc;
using UnkKraidList_l6015bc = base::UnkSList<UnkKraidListTag_l6015bc>;

struct UnkKraidTrackNode_l6015bc {
    u8 pad_0x0[0x8];
    u64 value;  // 0x8
    u8 pad_0x10[0x8];
    UnkKraidTrackNode_l6015bc* next;  // 0x18
};

struct UnkKraidTracks_l6015bc {
    u8 pad_0x0[0x8];
    u32 count;  // 0x8
    u8 pad_0xc[0x4];
    UnkKraidTrackNode_l6015bc* head;  // 0x10
    UnkKraidTrackNode_l6015bc* tail;  // 0x18
};

struct UnkKraidOther_l6015bc {
    u8 pad_0x0[0x20];
    UnkKraidTracks_l6015bc* tracks;  // 0x20
    UnkKraidList_l6015bc list;       // 0x28
};

struct UnkKraidAI_l6015bc {
    u8 pad_0x0[0x1140];
    s32 state;  // 0x1140
    u8 pad_0x1144[0x14a38 - 0x1144];
    u8 entry_0x14a38[0x68];
    u8 entry_0x14aa0[0x38];
    u8 entry_0x14ad8[0xa8];
    u8 entry_0x14b80[0x40];
    u8 entry_0x14bc0[0x8];
};
static_assert(__builtin_offsetof(UnkKraidAI_l6015bc, entry_0x14bc0) == 0x14bc0, "layout");

// Whether `value` is among the tracks ahead of `stop`, with its position in `index`.
static inline bool FindBefore_l6015bc(UnkKraidTracks_l6015bc* tracks, u64 value, u64 stop, u32& index) {
    bool found = false;
    index = 0;
    UnkKraidTrackNode_l6015bc* n = tracks->head;
    while (n && n->value != stop) {
        if (n->value == value)
            found = true;
        else
            ++index;
        if (found)
            break;
        n = n->next;
    }
    return found;
}

static inline s32 IndexOf_l6015bc(UnkKraidList_l6015bc* list, u64 v) {
    u32 i = 0;
    for (UnkKraidList_l6015bc::Node* n = list->head; n; n = n->next, ++i)
        if (n->value == v)
            return static_cast<s32>(i);
    return -1;
}

// Unlinks and frees the node holding `v`; nothing when it is not there.
static inline void Remove_l6015bc(UnkKraidList_l6015bc* list, u64 v) {
    UnkKraidList_l6015bc::Node* n = list->head;
    while (n && n->value != v)
        n = n->next;
    if (n)
        list->Remove(n);
}

void fn_71006015bc(UnkKraidAI_l6015bc* self, UnkKraidOther_l6015bc* other, u32 id, bool flag) {
    fn_710110bb4c(reinterpret_cast<UnkQuetzoaXAI_ovb0134*>(self), reinterpret_cast<UnkQuetzoaOther_ovb0134*>(other),
                  id, flag);
    UnkKraidList_l6015bc* list = &other->list;
    u32 index;
    if (FindBefore_l6015bc(other->tracks, reinterpret_cast<u64>(self->entry_0x14bc0),
                           reinterpret_cast<u64>(self->entry_0x14b80), index) &&
        index == 0 && IndexOf_l6015bc(list, reinterpret_cast<u64>(self->entry_0x14aa0)) >= 0) {
        Remove_l6015bc(list, reinterpret_cast<u64>(self->entry_0x14aa0));
        list->PushFront(reinterpret_cast<u64>(self->entry_0x14aa0));
    }
    UnkKraidTracks_l6015bc* tracks = other->tracks;
    if (self->state == 1) {
        // The tracks at the head of the list that are +0x14aa0 or +0x14a38.
        u32 leading = 0;
        bool ours = true;
        for (UnkKraidTrackNode_l6015bc* n = tracks->head; n && ours; n = n->next) {
            ours = n->value == reinterpret_cast<u64>(self->entry_0x14aa0) ||
                   n->value == reinterpret_cast<u64>(self->entry_0x14a38);
            if (ours)
                ++leading;
        }
        if (leading < 3)
            return;
        if (IndexOf_l6015bc(list, reinterpret_cast<u64>(self->entry_0x14ad8)) < 0)
            return;
        Remove_l6015bc(list, reinterpret_cast<u64>(self->entry_0x14ad8));
        list->PushFront(reinterpret_cast<u64>(self->entry_0x14ad8));
    } else {
        if (tracks->count != 1)
            return;
        u64 only = tracks->tail->value;
        if (only != reinterpret_cast<u64>(self->entry_0x14a38))
            return;
        if (IndexOf_l6015bc(list, only) < 0)
            return;
        Remove_l6015bc(list, only);
        list->PushBack(only);
    }
}
