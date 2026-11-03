// animtree::CAnimTreeDef unit (tools/tu_map.py): the copy constructor of a pair of string ids.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"

using base::global::CStrId;

struct UnkAnimTreeStrIdPair {
    CStrId first;
    CStrId second;
};

void fn_71002db084(UnkAnimTreeStrIdPair* self, const UnkAnimTreeStrIdPair* other) {
    new (&self->first) CStrId(other->first);
    new (&self->second) CStrId(other->second);
}
