// Copies of fn_7100d6ea28 (src/base/reflection/fn_7100b80bbc.cpp), which the binary
// repeats at other addresses: fn_7100242bcc, fn_7100120e3c.
// Clear of a hash map keyed by string ids whose nodes are also chained in a list: unless it is
// already empty, releases each node's key and frees the node, then empties the buckets and the
// list.
#include "base/UnkDestructors.h"
#include "base/global/CRntDictionary.h"
#include "types.h"

using UnkIdHashMap = base::global::CRntDictionary<base::UnkStrIdMember, u64>;

void fn_7100242bcc(UnkIdHashMap* self) { self->Clear(); }

void fn_7100120e3c(UnkIdHashMap* self) { self->Clear(); }
