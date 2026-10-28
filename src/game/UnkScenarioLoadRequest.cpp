// From unit 2622 of tools/tu_map.py (0x7100fc71ac-0x7100fc7f6c, no class hint): the load request
// the user data's owner fills before it starts a scenario load (src/game/UnkUserDataOwner.cpp).
// Names and layouts are placeholders.
// Holds fn_7100fc7920.
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

// A list of pointers in allocated nodes (src/game/CGameManager.cpp's UnkPtrList, as this unit
// reads it).
struct UnkPtrList;
struct UnkRequestListNode {
    void* value;
    UnkRequestListNode* next;
};
extern char g_unk_7101aa9ac0[];  // the list's base vtable (as src/game/CGameManager.cpp declares it)

struct UnkRequestList {
    void** vtable;
    // A union member: the original reloads the count after storing a new node's links.
    union {
        u32 count;
    };
    UnkRequestListNode* head;
    UnkRequestListNode* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    // Frees every node.
    void Clear() {
        for (UnkRequestListNode* n = head; n;) {
            UnkRequestListNode* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
    __attribute__((always_inline)) ~UnkRequestList() {
        vtable = reinterpret_cast<void**>(g_unk_7101aa9ac0 + 0x10);
        Clear();
    }
};

// One request (0x208 bytes): the scenario's strings and the blackboards it is given.
struct UnkScenarioLoadEntry {
    CRntString scenario;  // 0x0
    CRntString name;      // 0x30
    CRntString project;   // 0x60
    CRntString s2;        // 0x90
    CRntString s3;        // 0xc0
    UnkRequestList blackboards;  // 0xf0
    CRntString c;         // 0x120
    u8 unk_0x150[0x88];
    CRntString field_0x1d8;

    __attribute__((always_inline)) ~UnkScenarioLoadEntry() {}
};
static_assert(sizeof(UnkScenarioLoadEntry) == 0x208, "UnkScenarioLoadEntry placeholder layout");

struct UnkScenarioLoadRequest {
    u32 kind;
    bool flag;
    void (*fn)();
    base::UnkVector<UnkScenarioLoadEntry> entries;  // 0x10
    bool pending;  // 0x38
};
static_assert(__builtin_offsetof(UnkScenarioLoadRequest, pending) == 0x38, "UnkScenarioLoadRequest placeholder layout");

// Adds an entry at the end and returns it.
UnkScenarioLoadEntry* fn_7100fc77b0(base::UnkVector<UnkScenarioLoadEntry>* entries);

// Replaces the request with one entry built from the arguments and marks it pending.
void fn_7100fc7920(void* loader, u32 kind, const CRntString* scenario, const CRntString* name,
                   UnkPtrList* blackboards, bool a, void (*fn)(), const CRntString* c, CRntString* project,
                   CRntString* s2, CRntString* s3) {
    auto* self = static_cast<UnkScenarioLoadRequest*>(loader);
    self->flag = a;
    self->kind = kind;
    self->fn = fn;
    for (u64 i = 0; i < self->entries.count; ++i)
        self->entries.data[i].~UnkScenarioLoadEntry();
    self->entries.count = 0;
    UnkScenarioLoadEntry* e = fn_7100fc77b0(&self->entries);
    fn_710007eaa0(&e->scenario, scenario);
    fn_710007eaa0(&e->name, name);
    e->blackboards.Clear();
    for (const UnkRequestListNode* n = reinterpret_cast<const UnkRequestList*>(blackboards)->head; n; n = n->next) {
        auto* node = static_cast<UnkRequestListNode*>(
            e->blackboards.allocator->vf_0x40(sizeof(UnkRequestListNode), 8, e->blackboards.flags));
        if (!node)
            continue;
        node->value = n->value;
        node->next = nullptr;
        if (e->blackboards.count == 0) {
            e->blackboards.head = node;
            e->blackboards.tail = node;
            e->blackboards.count = 1;
        } else {
            e->blackboards.tail->next = node;
            node->next = nullptr;
            e->blackboards.tail = node;
            ++e->blackboards.count;
        }
    }
    fn_710007eaa0(&e->c, c);
    fn_710007eaa0(&e->project, project);
    fn_710007eaa0(&e->s2, s2);
    fn_710007eaa0(&e->s3, s3);
    self->pending = true;
}
