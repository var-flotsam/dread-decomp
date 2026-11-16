// CFlockingSwarmControllerComponent unit: adds an entry to the swarm's group at an index; when the
// group was empty (and the swarm is not in state 3), it flags every entry of the index's list and
// drops the index from the swarm's list of empty groups. Names and layouts are placeholders.
#include <algorithm>

#include "base/global/CRntVector.h"
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "../../game_s7/UnkIter.h"
#include "types.h"

struct UnkEntryList_fs {
    UnkEntry** data;
    u32 count;
};

struct UnkArray_fs {
    void* data;
    u32 count;
};

UnkEntryList_fs* fn_7100738c9c(UnkArray_fs* array, u32 index);

// A group: the entries in it (a base::global::CRntVector of pointers), then more.
struct UnkSwarmGroup_l73912c {
    base::global::CRntVector<UnkEntry*> entries;
};

// An index of an empty group, with a word.
struct UnkEmptyGroup_l73912c {
    u32 index;
    u32 field_0x4;
};

// Removes the element at `at`, moving the later ones down one by one.
template <class T>
static inline void EraseAt(base::global::CRntVector<T>* v, u32 at) {
    if (at < v->count) {
        for (u32 i = at; i < v->count - 1; ++i)
            v->data[i] = v->data[i + 1];
        v->count = v->count - 1;
    }
}

struct UnkFlockingSwarm_l73912c {
    u8 pad_0x0[0x140];
    UnkArray_fs field_0x140;  // the entry lists, by group index
    u8 pad_0x150[0x18];
    base::global::CRntVector<UnkEmptyGroup_l73912c> empty_groups;  // +0x168
    u8 pad_0x190[0x18];
    UnkArray_fs groups;  // +0x1a8
    u8 pad_0x1b8[0x1d8];
    u32 state;  // +0x390
};
static_assert(__builtin_offsetof(UnkFlockingSwarm_l73912c, empty_groups) == 0x168, "placeholder layout");
static_assert(__builtin_offsetof(UnkFlockingSwarm_l73912c, groups) == 0x1a8, "placeholder layout");
static_assert(__builtin_offsetof(UnkFlockingSwarm_l73912c, state) == 0x390, "placeholder layout");

void fn_710073912c(UnkFlockingSwarm_l73912c* self, UnkEntry* entry, u32 index) {
    auto* group = reinterpret_cast<UnkSwarmGroup_l73912c*>(fn_71007410a4(&self->groups, index));
    u32 previous = group->entries.count;
    group->entries.PushBack(entry);
    if (previous != 0 || self->state == 3)
        return;

    UnkEntryList_fs* list = fn_7100738c9c(&self->field_0x140, index);
    for (UnkEntry **it = list->data, **end = list->data + list->count; it != end; ++it)
        fn_7100d35a98(*it, true);

    auto& empty = self->empty_groups;
    UnkIter<UnkEmptyGroup_l73912c> first{empty.data};
    UnkIter<UnkEmptyGroup_l73912c> end{empty.data + empty.count};
    UnkIter<UnkEmptyGroup_l73912c> it =
        std::find_if(first, end, [index](const UnkEmptyGroup_l73912c& e) { return e.index == index; });
    if (it != end)
        EraseAt(&empty, it - first);
}
