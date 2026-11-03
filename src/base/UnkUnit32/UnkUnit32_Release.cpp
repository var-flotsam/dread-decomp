// From unit 32 of tools/tu_map.py (0x71000255c4-0x7100027774): the release of a resource reference
// (base/UnkStrIdPathPair.h: the string id, then the file-path id, then the counted resource, each
// left empty), its destructor (the release, then the members) and its copy assignment. Names and
// layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "types.h"

static inline void Drop(UnkCountedResource* r) {
    if (--r->ref_count == 0)
        fn_710001737c(r);
}

void fn_7100027044(UnkStrIdPathResource* self) {
    self->ids.id.~UnkStrIdMemberOf();
    self->ids.path.~UnkStrIdMemberOf();
    if (UnkCountedResource* r = self->resource) {
        Drop(r);
        self->resource = nullptr;
    }
}

// The same object as callers outside the unit name it.
struct UnkIdPair;

void fn_7100026f70(UnkIdPair* pair) {
    UnkStrIdPathResource* self = reinterpret_cast<UnkStrIdPathResource*>(pair);
    fn_7100027044(self);
    if (UnkCountedResource* r = self->resource)
        Drop(r);
    self->ids.path.~UnkStrIdMemberOf();
    self->ids.id.~UnkStrIdMemberOf();
}
