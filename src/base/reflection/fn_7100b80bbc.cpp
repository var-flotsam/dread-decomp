// Clear of a hash map keyed by string ids whose nodes are also chained in a list: unless it is
// already empty, releases each node's key and frees the node, then empties the buckets and the
// list. Three identical copies.
#include "base/UnkDestructors.h"
#include "base/global/CRntDictionary.h"
#include "types.h"

using UnkIdHashMap = base::global::CRntDictionary<base::UnkStrIdMember, u64>;

void fn_7100b80bbc(UnkIdHashMap* self) { self->Clear(); }
void fn_7100d6ea28(UnkIdHashMap* self) { self->Clear(); }
void fn_7100d6f8ac(UnkIdHashMap* self) { self->Clear(); }
