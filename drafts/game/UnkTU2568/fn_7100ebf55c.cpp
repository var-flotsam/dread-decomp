// Unit 2568: clears the table at +0x62e30 of pairs of owned slot-pool owners (deleted and nulled) and
// string ids (released) sharing one count; the arrays stay. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "types.h"
using base::UnkStrIdMember;

struct UnkClone_fn_71002880d4;
struct UnkClone_fn_71002b17a0;
template <class Tag>
struct UnkSlotPoolOwner_b64a2;
void fn_7100288028(UnkSlotPoolOwner_b64a2<UnkClone_fn_71002880d4>* self);
void fn_71002b16f4(UnkSlotPoolOwner_b64a2<UnkClone_fn_71002b17a0>* self);

struct UnkOwnerPair_lebf55c {
    UnkSlotPoolOwner_b64a2<UnkClone_fn_71002880d4>* first;
    UnkSlotPoolOwner_b64a2<UnkClone_fn_71002b17a0>* second;
};

struct UnkOwnerTable_lebf55c {
    UnkOwnerPair_lebf55c* pairs;
    UnkStrIdMember* ids;
    u32 count;
};

struct UnkTU2568_lebf55c {
    u8 pad_0x0[0x62e30];
    UnkOwnerTable_lebf55c table;
};

void fn_7100ebf55c(UnkTU2568_lebf55c* self) {
    UnkOwnerTable_lebf55c* table = &self->table;
    UnkOwnerPair_lebf55c* end = table->pairs + table->count;
    for (UnkOwnerPair_lebf55c* p = table->pairs; p != end; ++p) {
        if (UnkSlotPoolOwner_b64a2<UnkClone_fn_71002880d4>* first = p->first) {
            fn_7100288028(first);
            operator delete(first);
            p->first = nullptr;
        }
        if (UnkSlotPoolOwner_b64a2<UnkClone_fn_71002b17a0>* second = p->second) {
            fn_71002b16f4(second);
            operator delete(second);
            p->second = nullptr;
        }
    }
    if (table->ids && table->count)
        for (u64 i = 0; i < table->count; ++i)
            table->ids[i].~UnkStrIdMember();
    table->count = 0;
}
