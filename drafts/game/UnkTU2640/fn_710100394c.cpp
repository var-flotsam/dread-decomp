// Unit 2640: the begin of a reflected linked list: the iterator points at the list's first node and its
// cached value refers to that node's key and element. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "types.h"

struct UnkTU2640Elem {
    u64 field_0x0[6];
    ~UnkTU2640Elem();
};

struct UnkTU2640Node {
    u64 key;
    UnkTU2640Elem element;
};

struct UnkTU2640List {
    u8 pad_0x0[0x40];
    UnkTU2640Node* head;
};

struct UnkTU2640ListIter {
    UnkTU2640Node* node;
    base::UnkIterValue<UnkTU2640Elem, u64> current;
};

void fn_7100ff7360(char*);
UnkTU2640Elem::~UnkTU2640Elem() { fn_7100ff7360(reinterpret_cast<char*>(this)); }

void fn_710100394c(const UnkTU2640List* list, UnkTU2640ListIter* it) {
    it->node = list->head;
    if (it->node)
        it->current.SetRef(&it->node->key, &it->node->element);
    else
        it->current.Reset();
}
