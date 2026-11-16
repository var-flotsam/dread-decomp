#pragma once

// A singly linked list object: a vtable at 0, then the base::UnkLinkedList body at +8 (0x30
// bytes). Each element type has a vtable of its own, which the list's constructor and destructor
// store by hand, so the vtable is a plain pointer here and each list keeps its constructor and
// destructor (the destructor stores the vtable, then usually Clear or ClearDestroying). The count
// is UnkLinkedList's union member. Names are placeholders.

#include "base/UnkLinkedList.h"
#include "types.h"

namespace base {

// The word before the list body.
struct UnkListVtable {
    void* vtable;  // 0x0
};

template <class Node>
struct UnkVtableList : UnkListVtable, UnkLinkedList<Node> {};

}  // namespace base
