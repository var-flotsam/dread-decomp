// base::reflection's unit 271 (tools/tu_map.py): iterators of reflected linked lists (next node at
// +0x10, payload at +8) whose current element is kept in a two-state variant with a trivially
// destructible value (state 1 holds a value, state 2 a pair of pointers: the node and its payload).
// Advancing, assigning and starting one. Names and layouts are placeholders.
#include "types.h"

struct UnkListNode_b46a1_271 {
    u8 unk_0x0[0x10];
    UnkListNode_b46a1_271* next;
};

struct UnkListIter_b46a1_271 {
    UnkListNode_b46a1_271* node;
    union {
        u8 value[0x10];
        struct {
            void* first;
            void* second;
        } pair;
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

    __attribute__((always_inline)) void SetPair(void* first, void* second) {
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
        pair.first = first;
        pair.second = second;
    }

    __attribute__((always_inline)) void Set(UnkListNode_b46a1_271* n) {
        node = n;
        if (n)
            SetPair(n, reinterpret_cast<u8*>(n) + 8);
        else
            Reset();
    }

    __attribute__((always_inline)) UnkListIter_b46a1_271& Next() {
        Set(node->next);
        return *this;
    }

    __attribute__((always_inline)) UnkListIter_b46a1_271& Assign(const UnkListIter_b46a1_271& other) {
        if (this != &other)
            Set(other.node);
        return *this;
    }
};

struct UnkListOwner_b46a1_271 {
    u8 unk_0x0[0x40];
    UnkListNode_b46a1_271* head;
};

UnkListIter_b46a1_271& fn_71001f0e68(UnkListIter_b46a1_271* self) { return self->Next(); }
UnkListIter_b46a1_271& fn_71001f11b4(UnkListIter_b46a1_271* self) { return self->Next(); }

UnkListIter_b46a1_271& fn_71001f0f58(UnkListIter_b46a1_271* self, const UnkListIter_b46a1_271* other) {
    return self->Assign(*other);
}
UnkListIter_b46a1_271& fn_71001f0fb0(UnkListIter_b46a1_271* self, const UnkListIter_b46a1_271* other) {
    return self->Assign(*other);
}
UnkListIter_b46a1_271& fn_71001f12a4(UnkListIter_b46a1_271* self, const UnkListIter_b46a1_271* other) {
    return self->Assign(*other);
}
UnkListIter_b46a1_271& fn_71001f12fc(UnkListIter_b46a1_271* self, const UnkListIter_b46a1_271* other) {
    return self->Assign(*other);
}

void fn_71001f25e0(UnkListOwner_b46a1_271* owner, UnkListIter_b46a1_271* out) { out->Set(owner->head); }
