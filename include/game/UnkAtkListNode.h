#pragma once

#include "types.h"

// An intrusive list whose nodes sit at +0x288 of their owners; destroys every owner
// (fn_710122cf1c), reading the next node first.
struct UnkAtkListNode {
    UnkAtkListNode* prev;
    UnkAtkListNode* next;
};
