// CTunableTeamManager's unit (tools/tu_map.py unit 2738): empties the sorted list at +0x70, then adds to
// it, for every member of the list at +0x0 whose actor's component (+0x240) fn_710024fbc8 accepts, that
// actor's weak handle with the value fn_710110587c makes of the entry's. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CActorComponent.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

f32 fn_710110587c(void* component, f32 value);

struct UnkTeamActor_l1a6d50 {
    u8 pad_0x0[0x240];
    void* component;  // 0x240
};

// A weak handle to an actor with a value, kept in the lists below.
struct UnkTeamEntry_l1a6d50 {
    UnkWeakHandle* handle;
    f32 value;

    UnkTeamEntry_l1a6d50(UnkWeakTarget* target, f32 v) : handle(nullptr), value(v) {
        if (target)
            AssignRef(&handle, fn_7100d78378(target));
    }
    ~UnkTeamEntry_l1a6d50() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
};

// A node of either list: the member's handle and value first, the next node at +0x10.
struct UnkTeamNode_l1a6d50 {
    UnkWeakHandle* handle;
    f32 value;
    UnkTeamNode_l1a6d50* next;
};

// The member list (UnkTeamList in the unit's other file).
struct UnkTeamMembers_l1a6d50 {
    void** vtable;
    u32 count;
    UnkTeamNode_l1a6d50* head;
    UnkTeamNode_l1a6d50* tail;
    u8 pad_0x20[0x50];
};

// The sorted list (UnkTeamSortedList in the unit's other file).
struct UnkTeamRanked_l1a6d50 {
    void** vtable;
    u32 count;
    UnkTeamNode_l1a6d50* head;
    UnkTeamNode_l1a6d50* tail;
    u32 flags;
    base::IAllocator* allocator;
};

struct UnkTeamManager_l1a6d50 {
    UnkTeamMembers_l1a6d50 members;  // 0x0
    UnkTeamRanked_l1a6d50 sorted;    // 0x70
};

struct UnkTeamManager;
struct UnkTeamSortedList;
struct UnkTeamKey;
// Inserts a copy of the key in its place in the sorted list.
void CTunableTeamManager_InsertIntoSortedList(UnkTeamSortedList* self, const UnkTeamKey* key);

void CTeamManager_RebuildSortedList(UnkTeamManager* object) {
    UnkTeamManager_l1a6d50* self = reinterpret_cast<UnkTeamManager_l1a6d50*>(object);
    UnkTeamRanked_l1a6d50& entries = self->sorted;
    for (UnkTeamNode_l1a6d50* node = entries.head; node;) {
        UnkTeamNode_l1a6d50* next = node->next;
        if (UnkWeakHandle* handle = node->handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
        entries.allocator->vf_0x48(node);
        node = next;
    }
    entries.count = 0;
    entries.head = nullptr;
    entries.tail = nullptr;

    for (UnkTeamNode_l1a6d50* node = self->members.head; node; node = node->next) {
        void* component = reinterpret_cast<UnkTeamActor_l1a6d50*>(node->handle->target)->component;
        if (component && fn_710024fbc8(component)) {
            f32 value = fn_710110587c(component, node->value);
            UnkTeamEntry_l1a6d50 entry(node->handle ? node->handle->target : nullptr, value);
            CTunableTeamManager_InsertIntoSortedList(reinterpret_cast<UnkTeamSortedList*>(&entries),
                          reinterpret_cast<const UnkTeamKey*>(&entry));
        }
    }
}
