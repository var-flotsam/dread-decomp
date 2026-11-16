// Unit 2553 (CGameManager::CTunableGameManager): marks an actor as removed and queues it in the game
// manager's removal list (once), drops it from three other lists and, for an entity, tells the team
// manager and deactivates it. Names and layouts are placeholders.
#include "base/UnkLinkedList.h"
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

// The team manager singleton, reached through the GOT.
extern char* g_unk_71726b2398;

void fn_71011a73bc(char* manager, UnkRemovedValue_b60* entity);
char* fn_7100d7820c(char* p0, u32 p1, u32 p2, u32 p3, u32 p4);

// An actor as this sees it: reflected, with the removed flag at +0x1b5.
struct UnkRemovedActor_ldebc28 {
    u8 pad_0x0[0x1b5];
    bool removed;  // 0x1b5
};

struct UnkActorNode_ldebc28 {
    UnkRemovedValue_b60* value;
    UnkActorNode_ldebc28* next;
};

typedef base::UnkLinkedList<UnkActorNode_ldebc28> UnkActorList_ldebc28;

struct UnkRemovalManager_ldebc28 {
    u8 pad_0x0[0x2690];
    UnkActorList_ldebc28 list_0x2690;  // 0x2690
    u8 pad_0x26b8[0x48];
    UnkActorList_ldebc28 removed;  // 0x2700
    u8 pad_0x2728[0x48];
    UnkActorList_ldebc28 list_0x2770;  // 0x2770
    u8 pad_0x2798[0x48];
    UnkActorList_ldebc28 list_0x27e0;  // 0x27e0
};
static_assert(__builtin_offsetof(UnkRemovalManager_ldebc28, removed) == 0x2700, "layout");
static_assert(__builtin_offsetof(UnkRemovalManager_ldebc28, list_0x27e0) == 0x27e0, "layout");

// The position of `value` in `list`, or -1 when it is not there.
static inline s32 IndexOf_ldebc28(UnkActorList_ldebc28& list, UnkRemovedValue_b60* value) {
    u32 index = 0;
    for (UnkActorNode_ldebc28* node = list.head; node; node = node->next, ++index) {
        if (node->value == value)
            return static_cast<s32>(index);
    }
    return -1;
}

static inline void PushBack_ldebc28(UnkActorList_ldebc28& list, UnkRemovedValue_b60* value) {
    auto* node = static_cast<UnkActorNode_ldebc28*>(list.allocator->vf_0x40(sizeof(UnkActorNode_ldebc28), 8, list.flags));
    if (!node)
        return;
    node->value = value;
    node->next = nullptr;
    if (list.count == 0) {
        list.head = list.tail = node;
        list.count = 1;
    } else {
        list.tail->next = node;
        node->next = nullptr;
        list.tail = node;
        ++list.count;
    }
}

// Unlinks and frees the node holding `value`; nothing when it is not there.
static inline void Remove_ldebc28(UnkActorList_ldebc28& list, UnkRemovedValue_b60* value) {
    UnkActorNode_ldebc28* node = list.head;
    while (node && node->value != value)
        node = node->next;
    if (node)
        list.Remove(node);
}

static inline char* TeamManager_ldebc28() {
    char* instance = g_unk_71726b2398;
    if (!instance) {
        instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(instance);
        g_unk_71726b2398 = instance;
    }
    return instance;
}

void fn_7100debc28(UnkGameManager* manager, UnkRemovedValue_b60* value) {
    auto* self = reinterpret_cast<UnkRemovalManager_ldebc28*>(manager);
    auto* actor = reinterpret_cast<UnkRemovedActor_ldebc28*>(value);
    if (actor->removed)
        return;
    if (IndexOf_ldebc28(self->removed, value) >= 0)
        return;
    actor->removed = true;
    PushBack_ldebc28(self->removed, value);
    Remove_ldebc28(self->list_0x2690, value);
    Remove_ldebc28(self->list_0x27e0, value);
    Remove_ldebc28(self->list_0x2770, value);
    if (!value)
        return;
    auto* object = reinterpret_cast<base::reflection::UnkReflObject*>(value);
    if (fn_710009d50c(object->vf_0x0(), fn_7100d70c20())) {
        fn_71011a73bc(TeamManager_ldebc28(), value);
        fn_7100d7820c(reinterpret_cast<char*>(value), 1, 1, 1, 1);
    }
}
