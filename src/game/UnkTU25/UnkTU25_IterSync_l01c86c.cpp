// Unit 25: sync of an iterator whose current element is kept in a two-state variant: state 1 holds a
// string id and a file-path id (+0x8, +0x10 of the variant), released in reverse; state 2 the pair
// {iterator, node}. Sync makes it the pair when there is a node, else resets it. Names and layouts
// are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkPathIterList_01c86c {
    void* head;
};

struct UnkPathIter_01c86c {
    UnkPathIterList_01c86c list;
    void* node;  // 0x8
    union {
        struct {
            u8 unk_0x0[8];
            base::UnkStrIdMember id;
            base::UnkStrIdMemberOf<g_unk_7101d47200> path;
        } object;
        struct {
            void* first;
            void* second;
        } pair;
    };
    s32 state;  // 0x28

    void DestroyObject() {
        object.path.~UnkStrIdMemberOf();
        object.id.~UnkStrIdMemberOf();
    }

    void Reset() {
        switch (state) {
        case 1:
            DestroyObject();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }

    void SetPair(void* first, void* second) {
        s32* p = &state;
        switch (*p) {
        case 1:
            DestroyObject();
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

    void Sync() {
        if (void* n = node)
            SetPair(&list, n);
        else
            Reset();
    }
};
static_assert(__builtin_offsetof(UnkPathIter_01c86c, state) == 0x28, "state");

void fn_710001c86c(UnkPathIter_01c86c* self) {
    self->Sync();
}
