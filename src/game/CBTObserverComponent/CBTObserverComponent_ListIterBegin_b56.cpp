// Begin and end of the lists whose iterators src/game/CBTObserverComponent/
// CBTObserverComponent_ListIterSync_b55.cpp syncs (fn_7100ca8b88): a temporary iterator from the list
// head (or null) is synced out of line, assigned to the output, the output synced, and the
// temporary's variant reset (state 1 holds a {string id, array} object). Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

template <int Tag>
struct UnkListIter_b55;
void fn_7100ca8b88(UnkListIter_b55<0>* self);

struct UnkIdArrayIter_b56 {
    u8* node;
    union {
        struct {
            base::UnkStrIdMember id;
            base::UnkVector<u8> items;
        } object;
        struct {
            void* first;
            void* second;
        } pair;
    };
    s32 state;

    UnkIdArrayIter_b56() {}
    ~UnkIdArrayIter_b56() {}

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            object.items.Free();
            object.id.~UnkStrIdMemberOf();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }
};

struct UnkIdArrayListOwner_b56 {
    u8 unk_0x0[0x40];
    u8* head;
};

__attribute__((always_inline)) inline void UnkIdArrayListAt_b56(u8* node, UnkListIter_b55<0>* out) {
    UnkIdArrayIter_b56 it;
    it.node = node;
    it.state = 0;
    fn_7100ca8b88(reinterpret_cast<UnkListIter_b55<0>*>(&it));
    reinterpret_cast<UnkIdArrayIter_b56*>(out)->node = it.node;
    fn_7100ca8b88(out);
    it.Reset();
}

void fn_7100caa9e8(UnkIdArrayListOwner_b56* self, UnkListIter_b55<0>* out) {
    UnkIdArrayListAt_b56(self->head, out);
}

void fn_7100caaab4(UnkIdArrayListOwner_b56* self, UnkListIter_b55<0>* out) {
    UnkIdArrayListAt_b56(nullptr, out);
}
