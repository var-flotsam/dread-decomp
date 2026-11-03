// CActor's unit (tools/tu_map.py unit 341): Begin (slot 0x160) of a reflected linked list (the
// first node at +0x10) whose iterator holds the node, then a count of the nodes passed, then a
// cached (count, node) value with a trivially destructible content (as the list iterators of
// src/base/reflection/UnkUnit271ListIter_b46.cpp). Names and layouts are placeholders.
#include "types.h"

struct UnkActorListNode_b49a1;

struct UnkActorList_b49a1 {
    u8 unk_0x0[0x10];
    UnkActorListNode_b49a1* first;
};

struct UnkActorListIter_b49a1 {
    UnkActorListNode_b49a1* node;
    u32 index;
    union {
        u8 value[0x10];
        struct {
            const u32* index;
            UnkActorListNode_b49a1* node;
        } ref;
    };
    s32 state;

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }
    __attribute__((always_inline)) void SetRef(const u32* i, UnkActorListNode_b49a1* n) {
        s32* p = &state;
        switch (*p) {
        case 1:
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
        state = 2;
        ref.index = i;
        ref.node = n;
    }

    UnkActorListIter_b49a1(UnkActorListNode_b49a1* node, u32 index) : node(node), index(index), state(0) {
        if (node)
            SetRef(&this->index, node);
    }
    ~UnkActorListIter_b49a1() { Reset(); }
    UnkActorListIter_b49a1& operator=(const UnkActorListIter_b49a1& other) {
        if (this != &other) {
            index = other.index;
            node = other.node;
            if (node)
                SetRef(&index, node);
            else
                Reset();
        }
        return *this;
    }
};

void fn_7100240c44(const UnkActorList_b49a1* list, UnkActorListIter_b49a1* out) {
    *out = UnkActorListIter_b49a1(list->first, 0);
}
