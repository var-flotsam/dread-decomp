// Unit 25 of tools/tu_map.py: sets the count of an array of id and path pairs (the words of
// base/UnkStrIdPathPair.h; base::UnkVector's layout), first destroying the pairs from the new
// count on. Names and layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "types.h"

void fn_710001e62c(base::UnkVector<UnkStrIdPathPair>* self, u32 count) {
    for (u32 i = count; i < self->count; ++i)
        self->data[i].~UnkStrIdPathPair();
    self->count = count;
}
