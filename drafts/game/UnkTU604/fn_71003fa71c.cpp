// Unit 604 of tools/tu_map.py (no name hint): finds the group whose key is the actor's AI group
// (fn_71011277a4) and sets the flag at +0x1471c of every other member's data (fn_71003f92ac). The lists are
// those of UnkTU604_Lists_ovb0256.cpp. Names and layouts are placeholders.
#include "types.h"

struct UnkAIGroup_g;
UnkAIGroup_g* fn_71011277a4(void* src);

struct UnkListEntry_b256 {
    void* value;
    UnkListEntry_b256* next;
};

struct UnkListGroup_b256 {
    u8 pad_0x0[8];
    u32 count;                  // 0x8
    UnkListEntry_b256* head;    // 0x10
    u8 pad_0x18[0x58];
    void* key;                  // 0x70
    UnkListGroup_b256* next;    // 0x78
};

struct UnkListGroups_b256 {
    u8 pad_0x0[0x10];
    u32 count;                // 0x10
    UnkListGroup_b256* head;  // 0x18
};

char* fn_71003f92ac(void* value);

inline UnkListGroup_b256* GroupAt_l3fa71c(UnkListGroup_b256* n, u32 i) {
    for (u32 k = 0; k < i; ++k)
        n = n->next;
    return n;
}

inline UnkListEntry_b256* EntryAt_l3fa71c(UnkListEntry_b256* e, u32 i) {
    for (u32 k = 0; k < i; ++k)
        e = e->next;
    return e;
}

// The value of entry `j` of group `g`.
inline void* MemberAt_l3fa71c(UnkListGroups_b256* groups, u32 g, u32 j) {
    return EntryAt_l3fa71c(GroupAt_l3fa71c(groups->head, g)->head, j)->value;
}

struct UnkMemberData_l3fa71c {
    u8 pad_0x0[0x1471c];
    bool flag;  // 0x1471c
};

struct UnkAIRef_l3fa71c {
    u8 pad_0x0[0x10];
    void* source;  // 0x10
};

struct UnkActor_l3fa71c {
    u8 pad_0x0[0x398];
    UnkAIRef_l3fa71c* ai;  // 0x398
};

void fn_71003fa71c(UnkListGroups_b256* self, UnkActor_l3fa71c* actor) {
    if (!actor->ai || !actor->ai->source)
        return;
    void* key = fn_71011277a4(actor->ai->source);
    if (!key)
        return;
    u32 total = self->count;
    if (!total)
        return;
    UnkListGroup_b256* head = self->head;
    for (u32 i = 0; i != total; ++i) {
        if (GroupAt_l3fa71c(head, i)->key != key)
            continue;
        u32 n = GroupAt_l3fa71c(head, i)->count;
        for (u32 j = 0; j != n; ++j) {
            void* member = MemberAt_l3fa71c(self, i, j);
            if (member && member != actor)
                reinterpret_cast<UnkMemberData_l3fa71c*>(fn_71003f92ac(MemberAt_l3fa71c(self, i, j)))->flag = true;
        }
        return;
    }
}
