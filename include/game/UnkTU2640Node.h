#pragma once

#include "game/UnkTU2640Elem.h"

// The node is laid out as the pair it holds, followed by the link.
struct UnkTU2640Node {
    u64 key;
    UnkTU2640Elem element;
    UnkTU2640Node* next;
};
