// Unit 2640: the begin of a reflected keyed linked list: the iterator points at the list's first node and its
// current pair refers to that node's key and element, or is reset when the list is empty. Names and layouts
// are placeholders.
#include "game/UnkTU2640.h"
#include "game/UnkTU2640Elem.h"
#include "game/UnkTU2640List.h"
#include "game/UnkTU2640Node.h"
#include "types.h"

// A (key, element) pair whose destructor is out of line (one per list type).
template <void (*Destroy)(char*)>
struct UnkTU2640Pair {
    u64 key;
    UnkTU2640Elem element;

    ~UnkTU2640Pair() { Destroy(reinterpret_cast<char*>(this)); }
};

// The iterator: its node, and the current pair, which is empty, owns a copy, or refers to the node's key
// and element.
template <void (*Destroy)(char*)>
struct UnkTU2640ListIter {
    UnkTU2640Node* node;
    union {
        UnkTU2640Pair<Destroy> pair;
        struct {
            const u64* key;
            UnkTU2640Elem* element;
        } ref;
    };
    s32 state;  // 0 empty, 1 owns `pair`, 2 refers through `ref`

    ~UnkTU2640ListIter() { Reset(); }

    void Reset() {
        if (state == 2) {
        } else if (state == 1) {
            pair.~UnkTU2640Pair<Destroy>();
        } else {
            return;
        }
        state = 0;
    }

    void SetRef(const u64* key, UnkTU2640Elem* element) {
        Reset();
        state = 2;
        ref.key = key;
        ref.element = element;
    }

    void Begin(const UnkTU2640List* list) {
        node = list->head;
        if (node)
            SetRef(&node->key, &node->element);
        else
            Reset();
    }
};

void fn_710100394c(const UnkTU2640List* list, UnkTU2640ListIter<fn_7100ff7360>* it) { it->Begin(list); }
